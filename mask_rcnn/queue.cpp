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

#include <glog/logging.h>
#include <string>
#include <vector>
#include <boost/thread.hpp>
#include "queue.hpp"
#include "cnrt.h"  // NOLINT
#include "common_functions.hpp"

template<typename T>
Queue<T>::Queue() {
}

template<typename T>
void Queue<T>::push(const T& t) {
  queue_.push(t);
}

template<typename T>
bool Queue<T>::try_pop(T* t) {
  if (queue_.empty()) {
    return false;
  }

  *t = queue_.front();
  queue_.pop();
  return true;
}

template<typename T>
T Queue<T>::pop(const std::string& log_on_wait) {
  T t = queue_.front();
  queue_.pop();
  return t;
}

template<typename T>
bool Queue<T>::try_peek(T* t) {
  if (queue_.empty()) {
    return false;
  }

  *t = queue_.front();
  return true;
}

template<typename T>
T Queue<T>::peek() {
  return queue_.front();
}

template<typename T>
size_t Queue<T>::size() const {
  return queue_.size();
}

template class Queue<void**>;
template class Queue<std::shared_ptr<void*>>;
template class Queue<float*>;
template class Queue<std::shared_ptr<float>>;
template class Queue<std::vector<std::string>>;
template class Queue<std::vector<std::vector<float>>>;
template class Queue<std::shared_ptr<cnrtQueue_t>>;
template class Queue<std::map<cnrtParamDesc_t*, std::vector<int>>>;
template class Queue<InferenceTimeTrace*>;
template class Queue<std::shared_ptr<InferenceTimeTrace>>;
template class Queue<std::vector<std::pair<std::string, cv::Mat>>>;
