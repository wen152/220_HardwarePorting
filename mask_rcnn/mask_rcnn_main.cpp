/*
All modification made by Cambricon Corporation: © 2018--2019 Cambricon Corporation
All rights reserved.
All other contributions:
Copyright (c) 2014--2018, the respective contributors
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <algorithm>
#include <condition_variable>  // NOLINT
#include <iomanip>
#include <iosfwd>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <thread>  // NOLINT
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include "pipeline.hpp"
#include "data_provider.hpp"
#include "runner.hpp"
#include "mask_rcnn_processor.hpp"
#include "common_functions.hpp"
#include "mask_rcnn_main.hpp"

using std::map;
using std::pair;
using std::queue;
using std::string;
using std::stringstream;
using std::thread;
using std::vector;

DEFINE_int32(mask_dump, 1, "0 or 1, dump output images or not.");//默认参数，命令行可修改
DEFINE_string(mask_outputdir, ".", "The directoy used to save output images");

class MaskrcnnDataProvider : public DataProvider {
public:
  explicit MaskrcnnDataProvider(const std::string& meanvalue,
                        const std::queue<std::string>& images): DataProvider(meanvalue, images) {}
  void resizeMat(const cv::Mat& sample, cv::Mat& sample_resized) {
    cv::Mat sample_temp , sample_inputdim, sample_rgb, sample_temp_800, sample_temp_bgr;
    int pad_h = 0, pad_w = 0;
    int input_dim = this->inGeometry_.height;
    int channels = this->offlineDescripter()->inC();
    if (sample.size() != inGeometry_) {
      float img_w = sample.cols;
      float img_h = sample.rows;
      int pad_size = std::max(sample.cols, sample.rows);
      int dim_diff = std::abs(sample.cols - sample.rows);
      pad_h = sample.cols > sample.rows ? dim_diff / 2 : 0;
      pad_w = sample.cols < sample.rows ? dim_diff / 2 : 0;
      float img_scale = img_w < img_h ? (input_dim / img_h) : (input_dim / img_w);
      std::cout << "img_scale:" << img_scale << std::endl;
      //int new_w = std::floor(img_w * img_scale);
      //int new_h = std::floor(img_h * img_scale);
      //cv::resize(sample, sample_temp, cv::Size(new_w, new_h), CV_INTER_LINEAR);
      if (channels == 3) {
          sample_temp = cv::Mat(pad_size, pad_size, CV_8UC3, cv::Scalar(128, 128, 128)); 
      }
      if (channels == 4) {
          sample_temp = cv::Mat(pad_size, pad_size, CV_8UC4, cv::Scalar(128, 128, 128, 128)); 
      }
      sample.copyTo(sample_temp(cv::Range(pad_h, pad_h + sample.rows), 
			        cv::Range(pad_w, pad_w + sample.cols)));
      cv::resize(sample_temp, sample_temp_800, inGeometry_, CV_INTER_AREA);

      if (channels == 3) {
          //cv::cvtColor(sample_temp_800, sample_temp_bgr, cv::COLOR_BGR2RGB);
          sample_temp_800.convertTo(sample_temp, CV_32FC3, 1);
	        // cv::imwrite("mlu_pre.jpg", sample_temp_800);
      }
      if (channels == 4) {
          cv::cvtColor(sample_temp_800, sample_temp_bgr, cv::COLOR_BGR2RGBA);
          sample_temp_bgr.convertTo(sample_temp, CV_32FC4, 1);
      }
      cv::Rect select = cv::Rect(cv::Point(0, 0), inGeometry_);
      sample_resized = sample_temp(select);
    } else {
      printf("No padding!!!!\n");
      sample_resized = sample;
    }
  }
};

// TODO: czr- do not typedef here
typedef DataProvider DataProviderT;
typedef MaskrcnnDataProvider MaskrcnnDataProviderT;
typedef Runner RunnerT;
typedef PostProcessor PostProcessorT;
typedef Pipeline PipelineT;

int mask_rcnn_main() {
  {
    const char* env = getenv("log_prefix");
    if (!env || strcmp(env, "true") != 0) FLAGS_log_prefix = false;
  }
  //::google::InitGoogleLogging(argv[0]);
#ifndef GFLAGS_GFLAGS_H_
  namespace gflags = google;
#endif
  gflags::SetUsageMessage(
      "Do detection using mask_rcnn mode.\n"
      "Usage:\n"
      "    mask_rcnn_offline_multicore [FLAGS] model_file list_file\n");//--help打印帮助信息
  //gflags::ParseCommandLineFlags(&argc, &argv, true);//解析传入的参数
  //if (argc == 0) {
  //  gflags::ShowUsageWithFlagsRestrict(
  //      argv[0], "src/build/mask_rcnn/mask_rcnn_offline_multicore");
  //  return 1;
  //}

  int provider_num = 1, postprocessor_num = 1;

  if (FLAGS_mask_logdir != "") {
    FLAGS_log_dir = FLAGS_mask_logdir;//default=""
  } else {
    //  log to terminal's stderr if no log path specified
    FLAGS_alsologtostderr = 1;//未指定日志路径，打印到终端
  }

  // get device ids
  std::stringstream sdevice(FLAGS_mask_mludevice);//string mlu_device 
  vector<int> deviceIds_;
  std::string item;
  while (getline(sdevice, item, ',')) {
    int device = std::atoi(item.c_str());//转指针，*str-->int
    deviceIds_.push_back(device);
  }
  int totalThreads = FLAGS_mask_threads * deviceIds_.size();
//
  cnrtInit(0);//初始化运行时环境
  OfflineDescripter* offline_descripter = new OfflineDescripter(deviceIds_);
  offline_descripter->loadOfflinemodel(FLAGS_mask_offlinemodel,
                                   FLAGS_mask_channel_dup,
                                   FLAGS_mask_threads);

  ImageReader img_reader(FLAGS_mask_dataset_path, FLAGS_mask_images, totalThreads * provider_num);
  auto&& imageList = img_reader.getImageList();//图片路径
  int imageNum = img_reader.getImageNum();//图片数量
  if (imageNum < totalThreads)
    totalThreads = imageNum;//进程数<=图片数

  std::vector<std::thread*> stageThreads;
  std::vector<PipelineT*> pipelines;
  std::vector<DataProviderT*> providers;
  std::vector<PostProcessorT*> postprocessors;
  //单线程/多线程
  for (int i = 0; i < totalThreads; i++) {
    DataProviderT* provider;
    RunnerT* runner;
    PipelineT* pipeline;
    PostProcessorT* postprocessor;

    providers.clear();  // clear vector of last thread
    postprocessors.clear();
    for (int j = 0; j < provider_num; j++) {
      provider = new MaskrcnnDataProviderT(FLAGS_mask_meanvalue,
                                      imageList[provider_num * i + j]);
      providers.push_back(provider);
    }

    for (int j = 0; j < postprocessor_num; j++) {
      postprocessor = new MaskrcnnProcessor();
      postprocessors.push_back(postprocessor);
    }

    runner = new RunnerT(offline_descripter, i);
    pipeline = new PipelineT(providers, runner, postprocessors);

    stageThreads.push_back(new std::thread(&PipelineT::runParallel, pipeline));
    pipelines.push_back(pipeline);
  }

  for (int i = 0; i < stageThreads.size(); i++) {
    pipelines[i]->notifyAll();
  }
  Timer timer;
  for (int i = 0; i < stageThreads.size(); i++) {
    stageThreads[i]->join();
    delete stageThreads[i];
  }
  timer.log("Total execution time");

  float mluTime = 0;
  for (auto pipeline : pipelines) {
    mluTime += pipeline->runner()->runTime();
  }
  int batch_size = pipelines[0]->runner()->offlineDescripter()->inN();
  std::vector<InferenceTimeTrace> timetraces;
  for (auto iter : pipelines) {
    for (auto pP : iter->postProcessors()) {
      for (auto tc : pP->timeTraces()) {
        timetraces.push_back(tc);
      }
    }
  }
  printPerfTimeTraces(timetraces, batch_size, mluTime);
  saveResultTimeTrace(timetraces, (-1), (-1), (-1), imageNum, batch_size, mluTime);

  for (auto pipeline : pipelines)
    delete pipeline;

  cnrtDestroy();
}
