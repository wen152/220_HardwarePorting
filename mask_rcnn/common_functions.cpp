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
#include "common_functions.hpp"

using namespace boost::property_tree;

void printfMluTime(float mluTime) {
  LOG(INFO) << " execution time: " << mluTime;
}

void printfAccuracy(int imageNum, float acc1, float acc5) {
  LOG(INFO) << "Global accuracy : ";
  LOG(INFO) << "accuracy1: " << 1.0 * acc1 / imageNum << " ("
            << acc1 << "/" << imageNum << ")";
  LOG(INFO) << "accuracy5: " << 1.0 * acc5 / imageNum << " ("
            << acc5 << "/" << imageNum << ")";
}

void printPerf(int imageNum, float execTime, float mluTime, int batch_size, int threads) {
  int parallel_num = (threads > 32) ? 32 : threads;
  float hardwareFps = imageNum / mluTime * parallel_num *1e6;
  LOG(INFO) << "latency:  " << mluTime / imageNum * batch_size;
  LOG(INFO) << "throughput: " << hardwareFps;
}
std::string double2string(const double &value) {
  std::string s = std::to_string(value);
  auto end = s.find(".");
  s = s.substr(0, end+3);
  return s;
}


void saveResult(int imageNum, float top1, float top5, float meanAp,
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

  performance.put("average", double2string(latencytime));
  performance.put("throughput", double2string(hardwareFps));

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

void dumpJson(int imageNum, float top1, float top5, float meanAp,
              float latency, float throughput, float hw_time) {
  if (getenv("OUTPUT_JSON_FILE") == nullptr) return;
  const int TIME = -1;

  if (top1 != TIME)
    top1 = (1.0*top1 / imageNum) * 100;
  if (top5 != TIME)
    top5 = (1.0*top5 / imageNum) * 100;

  ptree output, output_list, accuracy, host_latency, hw_latency;
  accuracy.put("top1", double2string(top1));
  accuracy.put("top5", double2string(top5));
  accuracy.put("meanAp", double2string(meanAp));
  accuracy.put("f1", -1);
  accuracy.put("exact_match", -1);
  host_latency.put("average", double2string(latency / 1000));
  host_latency.put("throughput(fps)", double2string(throughput));
  hw_latency.put("average", double2string(hw_time));
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

std::vector<int> getTop5(std::vector<std::string> labels,
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

void readYUV(std::string name, cv::Mat img, int h, int w) {
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

cv::Mat readImage(std::string name, cv::Size size, bool yuvImg) {
  cv::Mat image;
  if (yuvImg) {
    image = convertYuv2Mat(name, size);
  } else {
    image = cv::imread(name, -1);
  }
  return image;
}

cv::Mat yuv420sp2Bgr24(cv::Mat yuv_image) {
    cv::Mat bgr_image(yuv_image.rows / 3 * 2,
                      yuv_image.cols,
                      CV_8UC3);
    cvtColor(yuv_image, bgr_image, CV_YUV420sp2BGR);
    return bgr_image;
}

cv::Mat convertYuv2Mat(std::string img_name, cv::Size inGeometry) {
  cv::Mat img = cv::Mat(inGeometry, CV_8UC1);
  readYUV(img_name, img, inGeometry.height, inGeometry.width);
  return img;
}

cv::Mat convertYuv2Mat(std::string img_name, int width, int height) {
  cv::Size inGeometry_(width, height);
  return convertYuv2Mat(img_name, inGeometry_);
}

TimeDuration_us getTimeDurationUs(const InferenceTimeTrace timetrace) {
  return std::chrono::duration_cast<TimeDuration_us>(
      timetrace.in_end - timetrace.in_start
      + timetrace.compute_end - timetrace.compute_start
      + timetrace.out_end - timetrace.out_start);
}

// return: [latency, fps]
// TODO: overflow risk here. Need change int to long/double
std::vector<float> getPerfDataFromTimeTraces(
    std::vector<InferenceTimeTrace> timetraces,
    int batchsize) {
  auto first_beg = timetraces[0].in_start;
  auto last_end = timetraces[0].out_end;
  std::vector<int> durations;
  for (auto tc : timetraces) {
    durations.push_back(getTimeDurationUs(tc).count());
    if (tc.in_start < first_beg)
      first_beg = tc.in_start;
    if (tc.out_end > last_end)
      last_end = tc.out_end;
  }
  int dur_average = 0;
  for (auto d : durations) {
    dur_average += d;
  }
  dur_average /= durations.size();
  auto totaldur = std::chrono::duration_cast<TimeDuration_us>(last_end - first_beg);
  float fps = batchsize * durations.size() * 1e6 / totaldur.count();
  std::vector<float> result;
  result.push_back(dur_average);
  result.push_back(fps);
  return result;
}

void printPerfTimeTraces(std::vector<InferenceTimeTrace> timetraces,
                         int batchsize, float mluTime) {
  auto result = getPerfDataFromTimeTraces(timetraces, batchsize);
  LOG(INFO) << "Throughput(fps): " << result[1];
  LOG(INFO) << "Latency(ms): " << result[0] / 1000;
  LOG(INFO) << "HardwareLatency(ms): " << mluTime / timetraces.size() / 1000;
  LOG(INFO) << "Inference count: " << timetraces.size() << " times";
}

void saveResultTimeTrace(std::vector<InferenceTimeTrace> timetraces, float top1, float top5,
                         float meanAp, int imageNum, int batchsize, float mluTime) {
  if (getenv("OUTPUT_JSON_FILE") == nullptr) return;
  auto result = getPerfDataFromTimeTraces(timetraces, batchsize);
  const int TIME = -1;
  float hw_time = mluTime / timetraces.size();

  if (top1 != TIME)
    top1 = (1.0*top1 / imageNum) * 100;
  if (top5 != TIME)
    top5 = (1.0*top5 / imageNum) * 100;

  ptree output, output_list, accuracy, host_latency, hw_latency;
  accuracy.put("top1", double2string(top1));
  accuracy.put("top5", double2string(top5));
  accuracy.put("meanAp", double2string(meanAp));
  accuracy.put("f1", double2string(-1));
  accuracy.put("exact_match", double2string(-1));
  host_latency.put("average", double2string(result[0] / 1000));
  host_latency.put("throughput(fps)", double2string(result[1]));
  hw_latency.put("average", double2string(hw_time / 1000));
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

// TODO: czr- why setDeviceId is defined here?
void setDeviceId(int deviceID) {
  unsigned devNum;
  CNRT_CHECK(cnrtGetDeviceCount(&devNum));
  if (deviceID >= 0) {
    CHECK_NE(devNum, 0) << "No device found";
    CHECK_LT(deviceID, devNum) << "Valid device count: " <<devNum;
  } else {
    LOG(FATAL) << "Invalid device number";
  }
  cnrtDev_t dev;
  CNRT_CHECK(cnrtGetDeviceHandle(&dev, deviceID));
  CNRT_CHECK(cnrtSetCurrentDevice(dev));
}

