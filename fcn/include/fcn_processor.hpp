#ifndef CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_PROCESSOR_HPP_
#define CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_PROCESSOR_HPP_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "glog/logging.h"
#include "fcn_post_processor.hpp"

using std::map;
using std::vector;
using std::string;
using std::pair;

template<class Dtype, template <class...> class Qtype>
class FcnProcessor : public PostProcessor<Dtype, Qtype> {
  public:
  FcnProcessor() { initClassesInfo(); }
  virtual ~FcnProcessor() {}
  void initClassesInfo();
  map<int, string> classInfoMap;
  vector<vector<int>> classRGBLabelVector;
};

#endif  // CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_PROCESSOR_HPP_
