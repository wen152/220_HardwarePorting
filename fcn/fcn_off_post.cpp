//#if defined(USE_MLU) && defined(USE_OPENCV)
#include <queue>
#include <string>
#include <sstream>
#include <thread> // NOLINT
#include <utility>
#include <vector>
#include <iomanip>
#include "cnrt.h" // NOLINT
#include "glog/logging.h"
#include "fcn_off_post.hpp"
#include "fcn_off_runner.hpp"
#include "command_option.hpp"
#include "fcn_common_functions.hpp"

using std::vector;
using std::string;
using std::pair;

template<class Dtype, template <class...> class Qtype>
std::mutex PostProcessor<Dtype, Qtype>::post_mutex_;

template<class Dtype, template <class...> class Qtype>
void FcnOffPostProcessor<Dtype, Qtype>::runParallel() {
  OffRunner<Dtype, Qtype> * infr = static_cast<OffRunner<Dtype, Qtype>*>(this->runner_);
  fcn_setDeviceId(infr->deviceId());

  // set parameters
  int dim_order[4] = {0, 3, 1, 2};
  cnrtDataType_t* output_data_type = nullptr;
  int outputNum = infr->outNum();
  cnrtGetOutputDataType(&output_data_type, &outputNum, infr->function());
  int temp_output_size = GET_OUT_TENSOR_SIZE(output_data_type[0], infr->outCount());
  unsigned int n, c, h, w;
  std::vector<unsigned int> shape(4, 1);
  int dimNum = 4;
  cnrtGetOutputDataShape((int**)&shape, &dimNum, 0, infr->function());  // NCHW
  n = shape[0];
  h = shape[1];
  w = shape[2];
  c = shape[3];
  unsigned int dim_shape[4] = {n, h, w, c};

  outCpuPtrs_ = MakeArray<float>(infr->outCount());
  outTrans_ = MakeArray<float>(infr->outCount());
  outCpuTempPtrs_ = MakeArray<char>(temp_output_size);

  while (true) {
    Timer postProcess;
    std::unique_lock<std::mutex> lock(PostProcessor<Dtype, Qtype>::post_mutex_);
    std::shared_ptr<Dtype> mluOutData = infr->popValidOutputData();
    if (mluOutData == nullptr) {
      lock.unlock();
      break;  // no more work
    }
    auto&& origin_img = infr->popValidInputNames();
    lock.unlock();

    Timer copyout;
    TimePoint t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < outputNum; i++) {
      cnrtMemcpy(outCpuTempPtrs_.get(),
                 mluOutData.get()[i],
                 infr->outputSizeS[i],
                 CNRT_MEM_TRANS_DIR_DEV2HOST);

      if (output_data_type[i] != CNRT_FLOAT32) {
        cnrtCastDataType(outCpuTempPtrs_.get(),
                         output_data_type[i],
                         outTrans_.get(),
                         CNRT_FLOAT32,
                         infr->outCount(),
                         NULL);
      } else {
        memcpy(outTrans_.get(), outCpuTempPtrs_.get(), infr->outputSizeS[i]);
      }
      cnrtTransDataOrder(outTrans_.get(),
                         CNRT_FLOAT32,
                         outCpuPtrs_.get(),
                         4,
                         (int*)dim_shape,
                         dim_order);
    }
    copyout.log("copyout time ...");
    TimePoint t2 = std::chrono::high_resolution_clock::now();
    auto timetrace = infr->popValidOutputTimeTraceData();
    timetrace->out_start = t1;
    timetrace->out_end = t2;
    this->appendTimeTrace(*timetrace);
    infr->pushFreeInputTimeTraceData(timetrace);
    infr->pushFreeOutputData(mluOutData);

    Timer dumpTimer;
    vector<cv::Mat> imgs;
    vector<string> img_names;
    if (FLAGS_fcn_dump && !FLAGS_fcn_perf_mode) {
      getImages(&imgs, &img_names, origin_img);
      getResults(img_names, outCpuPtrs_, c, h, w);
    }
  }
}

template<class Dtype, template <class...> class Qtype>
void FcnOffPostProcessor<Dtype, Qtype>::getResults(const vector<string>& origin_img,
                                                   const std::shared_ptr<float>& data,
                                                   const int& c,
                                                   const int& h,
                                                   const int& w) {
  std::vector<unsigned char> r_data(h * w, 0);
  cv::Mat r(h, w, CV_8UC1, r_data.data());

  for (int i = 0; i < origin_img.size(); i++) {
      string img_name = origin_img[i];
      if (img_name == "null") { return; }
      int pos = img_name.rfind(".");
      string name = img_name;
      if (pos > 0 && pos < name.size()) {
          name = name.substr(0, pos);
      }
      string img_num = name;
      for (int j = 0 ; j < h; j++) {
          for (int k = 0; k < w; k++) {
              float max_prob = 0.0;
              int max_index = 0;
              for (int m = 0; m < c; m++) {
                  int index = m * h * w + j * w + k + i * c * h * w;
                  if (data.get()[index] > max_prob) {
                      max_prob = data.get()[index];
                      max_index = m;
                  }
              }
              if (max_index >= this->classRGBLabelVector.size()) {
                  LOG(ERROR) << "max_index : " << max_index
                             << ", is more than " << this->classRGBLabelVector.size();
                  max_index = 0;
              }
              r_data[j * w + k] = this->classRGBLabelVector[max_index][0];
          }
      }
      cv::Mat mergedImage;
      cv::Mat rgb[1] = {r};
      cv::merge(rgb, 1, mergedImage);
      string output_image = FLAGS_fcn_outputdir + '/' + img_num + ".png";
      cv::imwrite(output_image, mergedImage);
  }
}


//template<class Dtype, template <class...> class Qtype>
// void FcnOffPostProcessor1<Dtype, Qtype>::getResults(const vector<string>& origin_img,
//                                                    const std::shared_ptr<float>& data,
//                                                    const int& c,
//                                                    const int& h,
//                                                    const int& w) {
//   std::vector<unsigned char> r_data(h * w, 0);
//   std::vector<unsigned char> g_data(h * w, 0);
//   std::vector<unsigned char> b_data(h * w, 0);
//   cv::Mat r(h, w, CV_8UC1, r_data.data());
//   cv::Mat g(h, w, CV_8UC1, g_data.data());
//   cv::Mat b(h, w, CV_8UC1, b_data.data());

//   for (int i = 0; i < origin_img.size(); i++) {
//       string img_name = origin_img[i];
//       if (img_name == "null") { return; }
//       int pos = img_name.rfind(".");
//       string name = img_name;
//       if (pos > 0 && pos < name.size()) {
//           name = name.substr(0, pos);
//       }
//       string img_num = name;
//       for (int j = 0 ; j < h; j++) {
//           for (int k = 0; k < w; k++) {
//               float max_prob = 0.0;
//               int max_index = 0;
//               for (int m = 0; m < c; m++) {
//                   int index = m * h * w + j * w + k + i * c * h * w;
//                   if (data.get()[index] > max_prob) {
//                       max_prob = data.get()[index];
//                       max_index = m;
//                   }
//               }
//               if (max_index >= this->classRGBLabelVector.size()) {
//                   LOG(ERROR) << "max_index : " << max_index
//                              << ", is more than " << this->classRGBLabelVector.size();
//                   max_index = 0;
//               }
//               r_data[j * w + k] = this->classRGBLabelVector[max_index][0];
//               g_data[j * w + k] = this->classRGBLabelVector[max_index][1];
//               b_data[j * w + k] = this->classRGBLabelVector[max_index][2];
//           }
//       }
//       cv::Mat mergedImage;
//       cv::Mat rgb[3] = {b, g, r};
//       cv::merge(rgb, 3, mergedImage);
//       string output_image = FLAGS_fcn_outputdir + '/' + img_num + ".png";
//       cv::imwrite(output_image, mergedImage);
//   }
// }

template<class Dtype, template <class...> class Qtype>
void FcnOffPostProcessor<Dtype, Qtype>::getImages(vector<cv::Mat>*imgs,
                                                  vector<string>*img_names,
                                                  vector<pair<string, cv::Mat>> origin_img) {
  for (auto&& img_name : origin_img) {
    if (img_name.first != "null") {
      cv::Mat img;
      img = img_name.second;
      int pos = img_name.first.find_last_of('/');
      string file_name(img_name.first.substr(pos + 1));
      imgs->push_back(img);
      img_names->push_back(file_name);
    }
  }
}

INSTANTIATE_OFF_CLASS(FcnOffPostProcessor);
//#endif  // defined(USE_MLU) && defined(USE_OPENCV)
