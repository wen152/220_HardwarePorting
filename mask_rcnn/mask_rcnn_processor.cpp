#include "glog/logging.h"
#include <queue>
#include <string>
#include <sstream>
#include <thread> // NOLINT
#include <utility>
#include <vector>
#include <iomanip>
#include "cnrt.h" // NOLINT
#include "mask_rcnn_processor.hpp"
#include "runner.hpp"
#include "command_option.hpp"
#include "common_functions.hpp"
#include <sys/stat.h>
#include <sys/types.h>

using std::pair;
using std::vector;
using std::string;
using std::pair;

std::mutex PostProcessor::post_mutex_;

DEFINE_double(mask_confidence, 0.05, "threshold");


void MaskrcnnProcessor::readLabels(std::vector<string>& label_name) {
  if (!FLAGS_mask_labels.empty()) {
    std::ifstream labels(FLAGS_mask_labels);
    string line;
    while (std::getline(labels, line)) {
      label_name.push_back(line);
    }
    labels.close();
  }
}

void MaskrcnnProcessor::runParallel() {
  setDeviceId(deviceId_);
  this->readLabels(this->label_name);

  int dim_order[4] = {0, 3, 1, 2};
  // TODO: czr-it's confusing that a micro is introduced to calculate tensor size. why?
  cnrtDataType_t* output_data_type = this->offlineDescripter()->getOutputDataType();
  int outputNum = this->offlineDescripter()->outputNum();

  outCpuPtrs_ = MakeArray<float*>(outputNum);
  outTrans_ = MakeArray<float*>(outputNum);
  outCpuTempPtrs_ = MakeArray<char*>(outputNum);


  for(int i = 0; i < outputNum; i++){
    auto outCount = this->offlineDescripter()->outCount(i);
    outCpuPtrs_.get()[i] = new float[outCount];
    outTrans_.get()[i] = new float[outCount];
    outCpuTempPtrs_.get()[i] = new char[GET_OUT_TENSOR_SIZE(output_data_type[i], outCount)];
  }


  // TODO: Hardcode thread_num to 4 here. Needs to wrap it in a class
  int TASK_NUM = 4;
  zl::ThreadPool tp(TASK_NUM);
  while (true) {
    std::unique_lock<std::mutex> lock(post_mutex_);
    auto output_boxing_data = this->offlineDescripter()->popValidOutputBoxingData(this->deviceId_);
    if (output_boxing_data == nullptr) {
      lock.unlock();
      break;  // no more data to process
    }
    auto mluOutData = output_boxing_data->getBuf();
    auto origin_img = output_boxing_data->getImageAndName();
    lock.unlock();

    TimePoint t1 = std::chrono::high_resolution_clock::now();
    for (int i=0; i < outputNum; i++) {
      auto outCount = this->offlineDescripter()->outCount(i);
      auto output_shape = this->offlineDescripter()->getOutputShape(i);
      int dim_shape[4] = {output_shape[0], output_shape[2], output_shape[3], output_shape[1]};

      cnrtMemcpy(outCpuTempPtrs_.get()[i],
                 mluOutData.get()[i],
                 this->offlineDescripter()->outputSizeS[i],
                 CNRT_MEM_TRANS_DIR_DEV2HOST);

      if (output_data_type[i] != CNRT_FLOAT32) {
        cnrtCastDataType(outCpuTempPtrs_.get()[i],
                         output_data_type[i],
                         outTrans_.get()[i],
                         CNRT_FLOAT32,
                         outCount,
                         nullptr);
      } else {
        memcpy(outTrans_.get()[i],
               outCpuTempPtrs_.get()[i],
               this->offlineDescripter()->outputSizeS[i]);
      }

      cnrtTransDataOrder(outTrans_.get()[i],
                         CNRT_FLOAT32,
                         outCpuPtrs_.get()[i],
                         4,
                         dim_shape,
                         dim_order);
    }
    
    TimePoint t2 = std::chrono::high_resolution_clock::now();
    auto time_stamp = output_boxing_data->getStamp();
    time_stamp->out_start = t1;
    time_stamp->out_end = t2;
    this->appendTimeTrace(*time_stamp);
    this->offlineDescripter()->pushFreeOutputBoxingData(output_boxing_data, this->deviceId_);

    Timer dumpTimer;
    auto final_boxes = getResults(origin_img, this->label_name);
  }
}

vector<vector<Mask>> MaskrcnnProcessor::getResults(vector<pair<string, cv::Mat>> origin_img, 
                                                   const vector<string>& labelToDisplayName) {
    int batch_size = this->offlineDescripter()->getInputShape()[0];
    auto mask_shape = this->offlineDescripter()->getOutputShape(0);
    int box_per_img = mask_shape[0] / batch_size;
    int mask_per_batch_size = this->offlineDescripter()->outCount(0) / batch_size;
    int box_per_batch_size = this->offlineDescripter()->outCount(1) / batch_size;
    int label_per_batch_size = this->offlineDescripter()->outCount(2) / batch_size;
    int score_per_batch_size = this->offlineDescripter()->outCount(3) / batch_size;
    vector<int> compression_param;
    compression_param.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_param.push_back(0);

    vector<vector<Mask>> final_boxes(this->offlineDescripter()->inN());
    vector<cv::Mat> imgs;
    vector<string> img_names;
    getImages(&imgs, &img_names, origin_img);
    for (int b = 0; b < batch_size; b++){
      float* mask = outCpuPtrs_.get()[0] + b * mask_per_batch_size;
      float* box = outCpuPtrs_.get()[1] + b * box_per_batch_size;
      float* label = outCpuPtrs_.get()[2] + b * label_per_batch_size;
      float* score =outCpuPtrs_.get()[3] + b * score_per_batch_size;

      vector<Mask> results;
      cv::Mat image = imgs[b];
      string name = img_names[b];
      vector<vector<cv::Point>> contours; 
      vector<cv::Vec4i> hierarchy;
      string image_id;
      cv::Mat mask_all;
      mask_all = cv::Mat::zeros(image.rows,image.cols,CV_8UC1);
      
      findNameId(img_names[b], name, image_id);
      string path = FLAGS_mask_outputdir + "/" + image_id;
      string res_name = path + "/mask_rcnn_" + name + ".jpg";
      string res_file = path + "/" + image_id + ".txt";
      string mask_path = FLAGS_mask_outputdir + "/mask";
      std::ofstream fileMap;
      if (FLAGS_mask_dump){        
        if(access(path.c_str(), F_OK) != 0){
          if (mkdir(path.c_str(), S_IRWXU) != 0)
            std::cout << "create " << path << "failed!!!" << std::endl;
        }
	if(access(mask_path.c_str(), F_OK) != 0){
	  if (mkdir(mask_path.c_str(), S_IRWXU) != 0)
            std::cout << "create " << path << "failed!!!" << std::endl;
        }
        fileMap.open(res_file);
      }
      float thresh = FLAGS_mask_confidence;

      int mask_size = mask_shape[2] * mask_shape[3];
      int mask_stride = mask_shape[1] * mask_shape[2] * mask_shape[3];

      for(int i = 0; i < box_per_img; i++){
        if(score[i] > thresh && label[i] > 0.0){
          float* selected_mask = mask + i * mask_stride + int(label[i]) * mask_size;
          float* selected_box = box + i * 4;
          Mask mask_box = map_box_to_ori_img(selected_box, selected_mask, image.size());
          auto bbox = mask_box.bbox;
          cv::Point p1(bbox[0], bbox[1]);
          cv::Point p2(bbox[2], bbox[3]);
          cv::rectangle(image, p1, p2, cv::Scalar(0, 255, 0), 2);
          cv::findContours(mask_box.mask_ori, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
          drawContours(image, contours, -1, cv::Scalar(0, 0, 255), 3);
          mask_box.score = score[i];
          mask_box.label = int(label[i]);
          results.push_back(mask_box);
          if (FLAGS_mask_dump){
            string mask_name = path + "/mask_" + std::to_string(i) + "_" + std::to_string(int(label[i])) + ".png";
            mask_all = mask_all + mask_box.mask_ori;
	    cv::imwrite(mask_name, mask_box.mask_ori, compression_param);
            fileMap << labelToDisplayName[static_cast<int>(label[i]) - 1] << " "
                << score[i] << " "
                << static_cast<float>(bbox[0]) << " "
                << static_cast<float>(bbox[1]) << " "
                << static_cast<float>(bbox[2] - bbox[0]) << " "
                << static_cast<float>(bbox[3] - bbox[1]) << " "
                << image.cols << " " << image.rows << " "
                << mask_name << std::endl;
          }
        }
      }
      string mask_all_name = path + "/mask.png";
      string mask_all_name_1 =  mask_path + "/mask_rcnn_" + name + ".png";
      cv::imwrite(mask_all_name, mask_all, compression_param);
      cv::imwrite(mask_all_name_1, mask_all, compression_param);
      final_boxes.push_back(results);
      if (FLAGS_mask_dump){
        fileMap.close();
        std::cout << "save image " << res_name << std::endl;
        cv::imwrite(res_name, image);
      }
    }
    return final_boxes;
}

Mask MaskrcnnProcessor::map_box_to_ori_img(float* box, float* mask, cv::Size ori_shape){
  int img_h = ori_shape.height;
  int img_w = ori_shape.width;
  std::vector<float> bbox(4);
  int mask_h = this->offlineDescripter()->getOutputShape(0)[2];
  int mask_w = this->offlineDescripter()->getOutputShape(0)[3];
  int input_dim = this->offlineDescripter()->inH();
  float img_scale = img_w < img_h ? (float(input_dim) / float(img_h)) : (float(input_dim) / float(img_w));

  bbox[0] = ((box[0] - (input_dim - img_scale * img_w) / 2.0) / img_scale);
  bbox[1] = ((box[1] - (input_dim - img_scale * img_h) / 2.0) / img_scale);
  bbox[2] = ((box[2] - (input_dim - img_scale * img_w) / 2.0) / img_scale);
  bbox[3] = ((box[3] - (input_dim - img_scale * img_h) / 2.0) / img_scale);
  cv::Mat mask_img(mask_h, mask_w, CV_32FC1, mask);
  Mask mask_res(bbox, mask_img, ori_shape);
  
  return mask_res;
}

void MaskrcnnProcessor::getImages(
    vector<cv::Mat>*imgs,
    vector<string>*img_names,
    vector<pair<string, cv::Mat>> origin_img) {
    for (auto img_name : origin_img) {
      if (img_name.first != "null") {
        cv::Mat img;
        if (FLAGS_mask_yuv) {
          cv::Size size = cv::Size(this->offlineDescripter()->inW(),
                                   this->offlineDescripter()->inH());
          img = yuv420sp2Bgr24(convertYuv2Mat(img_name.first, size));
        } else {
          img = img_name.second;;
        }
        imgs->push_back(img);
        img_names->push_back(img_name.first);
      }
    }
}
