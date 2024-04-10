#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic pop

#include <glog/logging.h> // NOLINT
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "command_option.hpp"
#include "fcn_common_functions.hpp"
#include "header.hpp"
using namespace boost::property_tree;

void fcn_printfMluTime(float mluTime) {
  LOG(INFO) << " execution time: " << mluTime;
}

void fcn_printfAccuracy(int imageNum, float acc1, float acc5) {
  LOG(INFO) << "Global accuracy : ";
  LOG(INFO) << "accuracy1: " << 1.0 * acc1 / imageNum << " ("
            << acc1 << "/" << imageNum << ")";
  LOG(INFO) << "accuracy5: " << 1.0 * acc5 / imageNum << " ("
            << acc5 << "/" << imageNum << ")";
}

void fcn_printPerf(int imageNum, float execTime, float mluTime, int batch_size, int threads) {
  int parallel_num = (threads > 32) ? 32 : threads;
  float hardwareFps = imageNum / mluTime * parallel_num *1e6;
  LOG(INFO) << "latency:  " << mluTime / imageNum * batch_size;
  LOG(INFO) << "throughput: " << hardwareFps;
}
std::string fcn_double2string(const double &value) {
  std::string s = std::to_string(value);
  auto end = s.find(".");
  s = s.substr(0, end+3);
  return s;
}


void fcn_saveResult(int imageNum, float top1, float top5, float meanAp,
                float hardwaretime, float endToEndTime, int batch_size, int threads) {
  if (getenv("OUTPUT_JSON_FILE") == nullptr) return;
  const int TIME = -1;
  float hardwareFps = -1;
  // float endToEndFps=-1;
  float latencytime = -1;
  int parallel_num = (threads > 32) ? 32 : threads;
  if (hardwaretime != TIME) {
    hardwareFps = imageNum / hardwaretime * parallel_num * 1e6;
    latencytime = hardwaretime / ((float)imageNum / batch_size);
  } else {
    hardwareFps = TIME;
  }

  if (top1 != TIME)
    top1 = (1.0*top1 / imageNum) * 100;
  if (top5 != TIME)
    top5 = (1.0*top5 / imageNum) * 100;

  ptree output, output_list, accuracy, performance;

  accuracy.put("top1", top1);
  accuracy.put("top5", top5);
  accuracy.put("meanAp", meanAp);
  accuracy.put("f1", -1);
  accuracy.put("exact_match", -1);

  performance.put("average", fcn_double2string(latencytime));
  performance.put("throughput", fcn_double2string(hardwareFps));

  output_list.put_child("Accuracy", accuracy);
  output_list.put_child("performance", performance);
  output.put_child("output", output_list);

  std::stringstream ss;
  write_json(ss, output);
  std::ofstream fout;
  std::string file = getenv("OUTPUT_JSON_FILE");
  fout.open(file);
  fout << ss.str() << std::endl;
  fout.close();
}

void fcn_dumpJson(int imageNum, float top1, float top5, float meanAp,
              float latency, float throughput, float hw_time) {
  if (getenv("OUTPUT_JSON_FILE") == nullptr) return;
  const int TIME = -1;

  if (top1 != TIME)
    top1 = (1.0*top1 / imageNum) * 100;
  if (top5 != TIME)
    top5 = (1.0*top5 / imageNum) * 100;

  ptree output, output_list, accuracy, host_latency, hw_latency;
  accuracy.put("top1", fcn_double2string(top1));
  accuracy.put("top5", fcn_double2string(top5));
  accuracy.put("meanAp", fcn_double2string(meanAp));
  accuracy.put("f1", -1);
  accuracy.put("exact_match", -1);
  host_latency.put("average", fcn_double2string(latency / 1000));
  host_latency.put("throughput(fps)", fcn_double2string(throughput));
  hw_latency.put("average", fcn_double2string(hw_time));
  output_list.put_child("Accuracy", accuracy);
  output_list.put_child("HostLatency(ms)", host_latency);
  if (hw_time != 0)
    output_list.put_child("HardwareCompute(ms)", hw_latency);
  output.put_child("Output", output_list);

  std::stringstream ss;
  write_json(ss, output);
  std::ofstream fout;
  std::string file = getenv("OUTPUT_JSON_FILE");
  fout.open(file);
  fout << ss.str() << std::endl;
  fout.close();
}
double PNSR(const cv::Mat& gt, const cv::Mat& data) {
  CHECK(gt.size() == data.size())
      << "size of gt_image and data doesn't match";

  double delta_square = 0.000;
  for (int i = 0; i < gt.rows; i++) {
    for (int j = 0; j < gt.cols; j++) {
      double delta = std::fabs(gt.at<float>(i, j)-data.at<float>(i, j));
      delta_square += std::pow(delta, 2);
    }
  }
  double rmse = std::sqrt(delta_square/(gt.cols*gt.rows));
  double pnsr = 20 * std::log10(255/rmse);
  return pnsr;
}
std::vector<int> fcn_getTop5(std::vector<std::string> labels,
                         std::string image,
                         float* data,
                         int count) {
  if (data == nullptr)
    throw "Data ptr is NULL";
  std::vector<int> index(5, 0);
  std::vector<float> value(5, data[0]);
  for (int i = 0; i < count; i++) {
    float tmp_data = data[i];
    int tmp_index = i;
    for (int j = 0; j < 5; j++) {
      if (data[i] > value[j]) {
        std::swap(value[j], tmp_data);
        std::swap(index[j], tmp_index);
      }
    }
  }
  std::stringstream stream;
  stream << "\n----- top5 for " << image << std::endl;
  for (int i = 0; i < 5; i++) {
    stream  << std::fixed << std::setprecision(4) << value[i] << " - "
            << labels[index[i]] << std::endl;
  }
  LOG(INFO) << stream.str();
  return index;
}

void fcn_readYUV(std::string name, cv::Mat img, int h, int w) {
  std::ifstream fin(name);
  unsigned char a;
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
      a = fin.get();
      img.at<char>(i, j) = a;
      fin.get();
    }
  fin.close();
}

cv::Mat fcn_readImage(std::string name, cv::Size size, bool yuvImg) {
  cv::Mat image;
  if (yuvImg) {
    image = fcn_convertYuv2Mat(name, size);
  } else {
    image = cv::imread(name, -1);
  }
  return image;
}

cv::Mat fcn_yuv420sp2Bgr24(cv::Mat yuv_image) {
    cv::Mat bgr_image(yuv_image.rows / 3 * 2,
                      yuv_image.cols,
                      CV_8UC3);
    cvtColor(yuv_image, bgr_image, CV_YUV420sp2BGR);
    return bgr_image;
}

cv::Mat fcn_convert_bgr_to_y(const cv::Mat& img) {
  std::vector<cv::Mat> channels;
  cv::split(img, channels);
  cv::Mat R;
  cv::Mat G;
  cv::Mat B;
  channels[0].convertTo(B, CV_64FC1);
  channels[1].convertTo(G, CV_64FC1);
  channels[2].convertTo(R, CV_64FC1);

  cv::Mat Y = 16 + (R*65.481+128.553*G+24.966*B)/255;

  return Y/255;
}

cv::Mat fcn_modcrop(const cv::Mat& imageData, int scale) {
  int width = imageData.size().width;
  int height = imageData.size().height;
  int modWidth = width % scale;
  int modHeight = height % scale;
  return imageData(cv::Rect(0, 0, width-modWidth, height-modHeight));
}

cv::Mat fcn_resizeAndConvertBgrToY(const std::pair<std::string, cv::Mat>& imageNameData,
                               const cv::Size& inGeometry, bool getGtImg) {
  std::string imagegName = imageNameData.first;
  cv::Mat imageData = imageNameData.second;
  cv::Mat Y;

  for (int i = 2; i < 5; i++) {
    if (imagegName.find(std::to_string(i)) != std::string::npos) {
      cv::Mat imageModCorp = fcn_modcrop(imageData, i);
      cv::Mat gt_y = fcn_convert_bgr_to_y(imageModCorp);
      cv::Mat imageDataResized = imresize(gt_y, inGeometry, true);
      if (!getGtImg) {
        cv::Size lowSize(inGeometry.width/i, inGeometry.height/i);
        cv::Mat lowResolution = imresize(imageDataResized, lowSize, true);
        Y = imresize(lowResolution, inGeometry, true);
      } else {
        Y = imageDataResized;
      }
    }
  }
  cv::Mat result;
  Y.convertTo(result, CV_32FC1);
  return result;
}

cv::Mat fcn_convertYuv2Mat(std::string img_name, cv::Size inGeometry) {
  cv::Mat img = cv::Mat(inGeometry, CV_8UC1);
  fcn_readYUV(img_name, img, inGeometry.height, inGeometry.width);
  return img;
}

cv::Mat fcn_scaleResizeCrop(cv::Mat sample, cv::Size inGeometry) {
    /**
     * Preprocess of input image for SegNet
     * 1.Set the short side value to inputSize
     * 2.Keep the aspect ration on long side
     * 3.Resize the input image to new size
     * 4.Center crop
     */
    int inputSize = inGeometry.width;
    auto img_w = sample.cols;
    auto img_h = sample.rows;
    auto new_w = img_h < img_w ? std::ceil(inputSize * img_w / img_h) : inputSize;
    auto new_h = img_h < img_w ? inputSize : std::ceil(inputSize * img_h / img_w);
    cv::Mat sample_resize;
    cv::resize(sample, sample_resize, cv::Size(new_w, new_h), cv::INTER_LINEAR);
    if (inGeometry.width > new_w || inGeometry.height > new_h) {
        LOG(ERROR) << "inGeometry_ size overrange inputSize * inputSize";
        exit(-1);
    }
    int left_x = std::floor((new_w - inGeometry.width) / 2);
    int top_y = std::floor((new_h - inGeometry.height) / 2);
    cv::Rect select = cv::Rect(cv::Point(left_x, top_y), inGeometry);
    return sample_resize(select);
}

cv::Mat fcn_convertYuv2Mat(std::string img_name, int width, int height) {
  cv::Size inGeometry_(width, height);
  return fcn_convertYuv2Mat(img_name, inGeometry_);
}

TimeDuration_us fcn_getTimeDurationUs(const InferenceTimeTrace timetrace) {
  return std::chrono::duration_cast<TimeDuration_us>(
      timetrace.in_end - timetrace.in_start
      + timetrace.compute_end - timetrace.compute_start
      + timetrace.out_end - timetrace.out_start);
}

// return: [latency, fps]
std::vector<double> fcn_getPerfDataFromTimeTraces(
    std::vector<InferenceTimeTrace> timetraces,
    int batchsize) {
  auto first_beg = timetraces[0].in_start;
  auto last_end = timetraces[0].out_end;
  std::vector<int> durations;
  for (auto tc : timetraces) {
    durations.push_back(fcn_getTimeDurationUs(tc).count());
    if (tc.in_start < first_beg)
      first_beg = tc.in_start;
    if (tc.out_end > last_end)
      last_end = tc.out_end;
  }
  double dur_average = 0;
  for (auto d : durations) {
    dur_average += d;
  }
  dur_average /= durations.size();
  auto totaldur = std::chrono::duration_cast<TimeDuration_us>(last_end - first_beg);
  double fps = batchsize * durations.size() * 1e6 / totaldur.count();
  std::vector<double> result;
  result.push_back(dur_average);
  result.push_back(fps);
  return result;
}

void fcn_printPerfTimeTraces(std::vector<InferenceTimeTrace> timetraces,
                         int batchsize, float mluTime) {
  auto result = fcn_getPerfDataFromTimeTraces(timetraces, batchsize);
  LOG(INFO) << "Throughput(fps): " << result[1];
  LOG(INFO) << "Latency(ms): " << result[0] / 1000;
  LOG(INFO) << "HardwareLatency(ms): " << mluTime / timetraces.size() / 1000;
  LOG(INFO) << "Inference count: " << timetraces.size() << " times";
}

void fcn_saveResultTimeTrace(std::vector<InferenceTimeTrace> timetraces, float top1, float top5,
                         float meanAp, int imageNum, int batchsize, float mluTime) {
  if (getenv("OUTPUT_JSON_FILE") == nullptr) return;
  auto result = fcn_getPerfDataFromTimeTraces(timetraces, batchsize);
  const int TIME = -1;
  float hw_time = mluTime / timetraces.size();

  if (top1 != TIME)
    top1 = (1.0*top1 / imageNum) * 100;
  if (top5 != TIME)
    top5 = (1.0*top5 / imageNum) * 100;

  ptree output, output_list, accuracy, host_latency, hw_latency;
  accuracy.put("top1", fcn_double2string(top1));
  accuracy.put("top5", fcn_double2string(top5));
  accuracy.put("meanAp", fcn_double2string(meanAp));
  accuracy.put("f1", fcn_double2string(-1));
  accuracy.put("exact_match", fcn_double2string(-1));
  host_latency.put("average", fcn_double2string(result[0] / 1000));
  host_latency.put("throughput(fps)", fcn_double2string(result[1]));
  hw_latency.put("average", fcn_double2string(hw_time / 1000));
  output_list.put_child("Accuracy", accuracy);
  output_list.put_child("HostLatency(ms)", host_latency);
  output_list.put_child("HardwareCompute(ms)", hw_latency);
  output.put_child("Output", output_list);

  std::stringstream ss;
  write_json(ss, output);
  std::ofstream fout;
  std::string file = getenv("OUTPUT_JSON_FILE");
  fout.open(file);
  fout << ss.str() << std::endl;
  fout.close();
}
