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

#include "blocking_queue.hpp"
#include <glog/logging.h>
#include <string>
#include <vector>
#include <map>
#include <boost/thread.hpp>
#include "cnrt.h" //NOLINT
#include "common_functions.hpp"

template<typename T>
class BlockingQueue<T>::sync {
  public:
  mutable boost::mutex mutex_;
  boost::condition_variable condition_;
};

template<typename T>
BlockingQueue<T>::BlockingQueue(): sync_(new sync()) {}

template<typename T>
void BlockingQueue<T>::push(const T& t) {
  boost::mutex::scoped_lock lock(sync_->mutex_);
  queue_.push(t);
  lock.unlock();
  sync_->condition_.notify_one();
}

template<typename T>
bool BlockingQueue<T>::try_pop(T* t) {
  boost::mutex::scoped_lock lock(sync_->mutex_);

  if (queue_.empty()) {
    return false;
  }

  *t = queue_.front();
  queue_.pop();
  return true;
}

template<typename T>
T BlockingQueue<T>::pop(const std::string& log_on_wait) {
  boost::mutex::scoped_lock lock(sync_->mutex_);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

  while (queue_.empty()) {
    if (!log_on_wait.empty()) {
      LOG_EVERY_N(INFO, 1000)<< log_on_wait;
    }
    sync_->condition_.wait(lock);
  }

  T t = queue_.front();
  queue_.pop();
  return t;
#pragma GCC diagnostic pop
}

template<typename T>
bool BlockingQueue<T>::try_peek(T* t) {
  boost::mutex::scoped_lock lock(sync_->mutex_);

  if (queue_.empty()) {
    return false;
  }

  *t = queue_.front();
  return true;
}

template<typename T>
T BlockingQueue<T>::peek() {
  boost::mutex::scoped_lock lock(sync_->mutex_);

  while (queue_.empty()) {
    sync_->condition_.wait(lock);
  }

  return queue_.front();
}

template<typename T>
size_t BlockingQueue<T>::size() const {
  boost::mutex::scoped_lock lock(sync_->mutex_);
  return queue_.size();
}

template class BlockingQueue<void**>;
template class BlockingQueue<std::shared_ptr<void*>>;
template class BlockingQueue<float*>;
template class BlockingQueue<std::shared_ptr<float>>;
template class BlockingQueue<std::vector<std::string>>;
template class BlockingQueue<std::vector<std::vector<float>>>;
template class BlockingQueue<std::shared_ptr<cnrtQueue_t>>;
template class BlockingQueue<std::shared_ptr<BoxingData>>;
template class BlockingQueue<std::map<cnrtParamDesc_t*, std::vector<int>>>;
template class BlockingQueue<std::vector<InputFeatures>>;
template class BlockingQueue<InferenceTimeTrace*>;
template class BlockingQueue<std::shared_ptr<InferenceTimeTrace>>;
template class BlockingQueue<std::vector<std::pair<std::string, cv::Mat>>>;
