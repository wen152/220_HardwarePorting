/*
All modification made by Cambricon Corporation: © 2019 Cambricon Corporation
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

#include "fcn_runner_strategy.hpp"
#include <gflags/gflags.h>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <thread> // NOLINT
#include <utility>
#include <iomanip>
#include <map>
#include <fstream>
//#ifdef USE_MLU
#include "cnrt.h" // NOLINT
//#endif
#include "fcn_blocking_queue.hpp"
#include "fcn_queue.hpp"
#include "fcn_post_processor.hpp"
#include "fcn_off_runner.hpp"
#include "fcn_common_functions.hpp"
#include "command_option.hpp"

template<class Dtype, template <class...> class Qtype>
void SimpleStrategy<Dtype, Qtype>::runParallel(OffRunner<Dtype, Qtype>* runner) {
#ifdef PINGPONG
#define RES_SIZE 2
#else
#define RES_SIZE 1
#endif

  // set device to runtime context binded device
  cnrtRuntimeContext_t ctxs[RES_SIZE];
  ctxs[0] = runner->runtimeContext();
  cnrtSetCurrentContextDevice(ctxs[0]);
  for (int i = 1; i < RES_SIZE; i++) {
    cnrtForkRuntimeContext(&ctxs[i], ctxs[0], NULL);
    cnrtSetCurrentContextDevice(ctxs[i]);
  }

  cnrtQueue_t queue[RES_SIZE];
  cnrtNotifier_t notifierBeginning[RES_SIZE];
  cnrtNotifier_t notifierEnd[RES_SIZE];

  for (int i = 0; i < RES_SIZE; i++) {
    // this queue is for sync copy, not for async copy
    if (!FLAGS_fcn_async_copy) {
      CHECK(cnrtCreateQueue(&queue[i]) == CNRT_RET_SUCCESS)
            << "CNRT create queue error, thread_id " << runner->threadId();
    }
    cnrtCreateNotifier(&notifierBeginning[i]);
    cnrtCreateNotifier(&notifierEnd[i]);
  }
  float eventInterval[RES_SIZE] = {0};
  std::shared_ptr<Dtype> mluInData[RES_SIZE];
  std::shared_ptr<Dtype> mluOutData[RES_SIZE];
  TimePoint timepoints[RES_SIZE];

  auto do_pop = [&](int index, void **param) {
    mluInData[index] = runner->popValidInputData();
    // this queue is for async copy, created in data provider
    if (FLAGS_fcn_async_copy) {
      queue[index] = runner->popValidAsyncCopyQueue();
    }

    if ( mluInData[index] == nullptr )
      return false;
    mluOutData[index] = runner->popFreeOutputData();
    for (int i = 0; i < runner->inBlobNum(); i++) {
      param[i] = mluInData[index].get()[i];
    }
    for (int i = 0; i < runner->outBlobNum(); i++) {
      param[runner->inBlobNum() + i] = mluOutData[index].get()[i];
    }

    return true;
  };

  auto do_invoke = [&](int index, void** param) {
    cnrtPlaceNotifier(notifierBeginning[index], queue[index]);
    int channel = runner->threadId() / runner->deviceSize() % 4;
    cnrtSetCurrentChannel((cnrtChannelType_t)channel);
    CNRT_CHECK(cnrtInvokeRuntimeContext(ctxs[index],
               param, queue[index], nullptr));
    timepoints[index] = std::chrono::high_resolution_clock::now();
  };

  auto do_sync = [&](int index) {
    cnrtPlaceNotifier(notifierEnd[index], queue[index]);
    if (cnrtSyncQueue(queue[index]) == CNRT_RET_SUCCESS) {
      cnrtNotifierDuration(notifierBeginning[index],
             notifierEnd[index], &eventInterval[index]);
      runner->setRunTime(runner->runTime() + eventInterval[index]);
      printfMluTime(eventInterval[index]);
    } else {
      LOG(ERROR) << " SyncQueue error";
    }
    runner->pushValidOutputData(mluOutData[index]);
    runner->pushFreeInputData(mluInData[index]);
    // this queue is for async copy
    if (FLAGS_fcn_async_copy) {
      runner->pushFreeAsyncCopyQueue(queue[index]);
    }
    TimePoint t2 = std::chrono::high_resolution_clock::now();
    auto timetrace = runner->popValidInputTimeTraceData();
    timetrace->compute_start = timepoints[index];
    timetrace->compute_end = t2;
    runner->pushValidOutputTimeTraceData(timetrace);
  };

#ifdef PINGPONG
  bool pong_valid = false;
  while (true) {
    void* param[runner->inBlobNum() + runner->outBlobNum()];

    // pop - ping
    Timer runner_timer;
    if (do_pop(0, static_cast<void **>(param)) == false) {
      if (pong_valid)
        do_sync(1);
      break;
    }

    // invoke - ping
    do_invoke(0, static_cast<void **>(param));

    // sync - pong
    if (pong_valid)
      do_sync(1);

    // pop - pong
    if (do_pop(1, static_cast<void **>(param)) == false) {
      do_sync(0);
      break;
    }

    // invoke - pong
    do_invoke(1, static_cast<void **>(param));
    pong_valid = true;

    // sync - ping
    do_sync(0);
    runner_timer.log("compute time ...");
  }
#else
  while (true) {
    void* param[runner->inBlobNum() + runner->outBlobNum()];
    // pop - ping
    Timer runner_timer;
    if (do_pop(0, static_cast<void **>(param)) == false) {
      break;
    }
    // invoke - ping
    do_invoke(0, static_cast<void **>(param));

    // sync - ping
    do_sync(0);
    runner_timer.log("compute time ...");
  }
#endif

  for (int i = 0; i < RES_SIZE; i++) {
    cnrtDestroyNotifier(&notifierBeginning[i]);
    cnrtDestroyNotifier(&notifierEnd[i]);
    // this queue is for sync copy, not for async copy
    if (!FLAGS_fcn_async_copy) {
      cnrtDestroyQueue(queue[i]);
    }
  }

  // tell postprocessor to exit
}

template<class Dtype, template <class...> class Qtype>
void FlexibleStrategy<Dtype, Qtype>::runParallel(OffRunner<Dtype, Qtype>* runner) {
  fcn_setDeviceId(runner->deviceId());
  cnrtQueue_t queue = runner->queue();
  CHECK(cnrtCreateQueue(&queue) == CNRT_RET_SUCCESS)
        << "CNRT create queue error, thread_id " << runner->threadId();
  runner->setQueue(queue);
  // initialize function memory
  cnrtNotifier_t notifierBeginning, notifierEnd;
  cnrtCreateNotifier(&notifierBeginning);
  cnrtCreateNotifier(&notifierEnd);
  float eventInterval = 0;

  while (true) {
    Timer runner_timer;
    std::shared_ptr<Dtype> mluInData = runner->popValidInputData();
    if ( mluInData == nullptr ) break;  // no more images

    std::shared_ptr<Dtype> mluOutData = runner->popFreeOutputData();
    const int param_len = runner->inBlobNum() + runner->outBlobNum();
    /* Dtype param[runner->inBlobNum() + runner->outBlobNum()]; */
    void* param[param_len];
    for (int i = 0; i < runner->inBlobNum(); i++) {
      param[i] = mluInData.get()[i];
    }
    for (int i = 0; i < runner->outBlobNum(); i++) {
      param[runner->inBlobNum() + i] = mluOutData.get()[i];
    }

#if defined(CROSS_COMPILE) || defined(CROSS_COMPILE_ARM64)
    struct timeval tpend, tpstart;
    gettimeofday(&tpstart, NULL);
#endif
    cnrtPlaceNotifier(notifierBeginning, runner->queue());
    CNRT_CHECK(cnrtInvokeRuntimeContext(runner->runtimeContext(),
               param, runner->queue(), nullptr));
    cnrtPlaceNotifier(notifierEnd, runner->queue());
#if defined(CROSS_COMPILE) || defined(CROSS_COMPILE_ARM64)
    gettimeofday(&tpend, NULL);
#endif

    if (cnrtSyncQueue(runner->queue()) == CNRT_RET_SUCCESS) {
      cnrtNotifierDuration(notifierBeginning, notifierEnd, &eventInterval);
#if defined(CROSS_COMPILE) || defined(CROSS_COMPILE_ARM64)
      eventInterval = 1000000 * (tpend.tv_sec - tpstart.tv_sec) +
        tpend.tv_usec - tpstart.tv_usec;
#endif
      runner->setRunTime(runner->runTime() + eventInterval);
      printfMluTime(eventInterval);
    } else {
      LOG(ERROR) << " SyncQueue error";
    }
    runner->pushValidOutputData(mluOutData);
    runner->pushFreeInputData(mluInData);
    runner_timer.log("compute time ...");
  }

  cnrtDestroyNotifier(&notifierBeginning);
  cnrtDestroyNotifier(&notifierEnd);
  // tell postprocessor to exit
}
INSTANTIATE_OFF_CLASS(RunnerStrategy);
INSTANTIATE_OFF_CLASS(SimpleStrategy);
INSTANTIATE_OFF_CLASS(FlexibleStrategy);
