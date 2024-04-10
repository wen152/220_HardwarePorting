/*
All modification made by Cambricon Corporation: © 2018 Cambricon Corporation
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

#include "glog/logging.h"
#include "clas_processor.hpp"
#include "fcn_runner.hpp"
#include "fcn_common_functions.hpp"
#include "command_option.hpp"

template <class Dtype, template <class...> class Qtype>
void ClassPostProcessor<Dtype, Qtype>::readLabels(std::vector<std::string>* labels) {
  if (!FLAGS_fcn_labels.empty()) {
    std::ifstream file(FLAGS_fcn_labels);
    if (file.fail())
      LOG(FATAL) << "failed to open labels file!";

    std::string line;
    while (getline(file, line)) {
      labels->push_back(line);
    }
    file.close();
    CHECK_EQ(this->outCount_ / this->outN_, labels->size())
        << "the number of classified objects is not equal to output of net";
  }
}

template <class Dtype, template <class...> class Qtype>
void ClassPostProcessor<Dtype, Qtype>::updateResult(
    const std::vector<std::pair<std::string, cv::Mat>>& origin_img,
    const std::vector<std::string>& labels,
    float* outCpuPtr) {
  for (int i = 0; i < this->outN_; i++) {
    auto image = origin_img[i].first;
    if (image == "null") break;

    this->total_++;
    if (image.find_last_of(" ") != -1) {
      image = image.substr(0, image.find(" "));
    }
    std::vector<int> vtrTop5 = fcn_getTop5(labels,
                                       image,
                                       outCpuPtr + i * this->outCount_ / this->outN_,
                                       this->outCount_ / this->outN_);
    image = origin_img[i].first;
    if (image.find(" ") != std::string::npos) {
      image = image.substr(image.find(" "));
    }

    int labelID = atoi(image.c_str());
    for (int i = 0; i < 5; i++) {
      if (vtrTop5[i] == labelID) {
        this->top5_++;
        if (i == 0)
          this->top1_++;
        break;
      }
    }
  }
}

template <class Dtype, template <class...> class Qtype>
void ClassPostProcessor<Dtype, Qtype>::printClassResult() {
  LOG(INFO) << "Accuracy thread id : " << this->runner_->threadId();
  LOG(INFO) << "accuracy1: " << 1.0 * this->top1_ / this->total_ << " ("
            << this->top1_ << "/" << this->total_ << ")";
  LOG(INFO) << "accuracy5: " << 1.0 * this->top5_ / this->total_ << " ("
            << this->top5_ << "/" << this->total_ << ")";
}

INSTANTIATE_ALL_CLASS(ClassPostProcessor);
