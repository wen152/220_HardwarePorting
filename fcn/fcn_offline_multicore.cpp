//#if defined(USE_MLU) && defined(USE_OPENCV)
#include <sys/time.h>
#include <gflags/gflags.h>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include "fcn_pipeline.hpp"
#include "off_data_provider.hpp"
#include "fcn_off_runner.hpp"
#include "fcn_off_post.hpp"
#include "fcn_common_functions.hpp"
#include "simple_interface.hpp"
#include "fcn_offline_multicore.hpp"

DEFINE_int32(fcn_dump, 1, "0 or 1, dump output images or not.");
DEFINE_string(fcn_outputdir, ".", "The directory used to save output images and txt.");

typedef DataProvider<void*, BlockingQueue> DataProviderT;
typedef OffDataProvider<void*, BlockingQueue> OffDataProviderT;
typedef OffRunner<void*, BlockingQueue> OffRunnerT;
typedef PostProcessor<void*, BlockingQueue> PostProcessorT;
typedef FcnOffPostProcessor<void*, BlockingQueue> FcnOffPostProcessorT;
typedef Pipeline<void*, BlockingQueue> PipelineT;

int fcn_main() {
  {
    const char * env = getenv("log_prefix");
    if (!env || strcmp(env, "true") != 0)
      FLAGS_log_prefix = false;
  }
  //::google::InitGoogleLogging(argv[0]);
  // Print output to stderr (while still logging)
  FLAGS_alsologtostderr = 1;
#ifndef GFLAGS_GFLAGS_H_
  namespace gflags = google;
#endif
  gflags::SetUsageMessage("Do detection using FCN mode.\n"
        "Usage:\n"
        "    fcn_offline_multicore [FLAGS]\n");
  //gflags::ParseCommandLineFlags(&argc, &argv, true);
  //if (argc == 0) {
  //  gflags::ShowUsageWithFlagsRestrict(argv[0], "examples/offline/fcn8s/fcn_offline_multicore");
  //  return 1;
  //}

  if (FLAGS_fcn_logdir != "") {
    FLAGS_log_dir = FLAGS_fcn_logdir;
  } else {
    //  log to terminal's stderr if no log path specified
    FLAGS_alsologtostderr = 1;
  }

  auto& simpleInterface = SimpleInterface::getInstance();
  int provider_num = 1, postprocessor_num = 1;
  simpleInterface.setFlag(true);
  provider_num = SimpleInterface::data_provider_num_;
  postprocessor_num = SimpleInterface::postProcessor_num_;

  std::stringstream sdevice(FLAGS_fcn_mludevice);
  std::vector<int> deviceIds_;
  std::string item;
  while (getline(sdevice, item, ',')) {
    int device = std::atoi(item.c_str());
    deviceIds_.push_back(device);
  }
  int totalThreads = FLAGS_fcn_threads * deviceIds_.size();
  cnrtInit(0);
  simpleInterface.loadOfflinemodel(FLAGS_fcn_offlinemodel,
                                   deviceIds_,
                                   FLAGS_fcn_channel_dup,
                                   FLAGS_fcn_threads);

  ImageReader img_reader(FLAGS_fcn_dataset_path, FLAGS_fcn_images, totalThreads * provider_num);
  auto&& imageList = img_reader.getImageList();
  int imageNum = img_reader.getImageNum();
  if (imageNum < totalThreads) { totalThreads = imageNum; }

  std::vector<std::thread*> stageThreads;
  std::vector<PipelineT*> pipelines;
  std::vector<DataProviderT*> providers;
  std::vector<PostProcessorT*> postprocessors;
  for (int i = 0; i < totalThreads; i++) {
    providers.clear();  // clear vector of last thread
    postprocessors.clear();
    for (int j = 0; j < provider_num; j++) {
      DataProviderT* provider = new OffDataProviderT(FLAGS_fcn_meanvalue,
                                      imageList[provider_num * i + j]);
      providers.push_back(provider);
    }

    for (int j = 0; j < postprocessor_num; j++) {
      PostProcessorT* postprocessor = new FcnOffPostProcessorT();
      postprocessors.push_back(postprocessor);
    }

    auto dev_runtime_contexts = simpleInterface.get_runtime_contexts();
    int index = i % deviceIds_.size();
    int thread_id = i / deviceIds_.size();
    OffRunnerT* runner = new OffRunnerT(dev_runtime_contexts[index][thread_id], i);
    PipelineT* pipeline = new PipelineT(providers, runner, postprocessors);

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
  int batch_size = pipelines[0]->runner()->n();
  std::vector<InferenceTimeTrace> timetraces;
  for (auto iter : pipelines) {
    for (auto pP : iter->postProcessors()) {
      for (auto tc : pP->timeTraces()) {
        timetraces.push_back(tc);
      }
    }
  }
  fcn_printPerfTimeTraces(timetraces, batch_size, mluTime);
  fcn_saveResultTimeTrace(timetraces, (-1), (-1), (-1), imageNum, batch_size, mluTime);

  for (auto pipeline : pipelines)
    delete pipeline;
  simpleInterface.destroyRuntimeContext();

  cnrtDestroy();
}


//#endif  // defined(USE_MLU) && defined(USE_OPENCV)
