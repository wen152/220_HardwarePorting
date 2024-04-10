#ifndef CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_OFF_POST_HPP_
#define CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_OFF_POST_HPP_
#include "fcn_processor.hpp"
#include "fcn_threadPool.hpp"
#include "simple_interface.hpp"

template<class Dtype, template <class...> class Qtype>
class FcnOffPostProcessor: public FcnProcessor<Dtype, Qtype> {
  public:
    FcnOffPostProcessor() {}
  virtual void runParallel();

  private:
    void getResults(const vector<string>& origin_img,
                    const std::shared_ptr<float>& data,
                    const int& c,
                    const int& h,
                    const int& w);
    void getImages(vector<cv::Mat>*imgs,
                   vector<string>*img_names,
                   vector<pair<string, cv::Mat>> origin_img);
    template<typename T>
    inline std::shared_ptr<T> MakeArray(int size) {
        return std::shared_ptr<T>( new T[size],
            [](T *p ) {
                delete[] p;
            });
    }

  private:
   std::shared_ptr<float> outCpuPtrs_;
   std::shared_ptr<float> outTrans_;
   std::shared_ptr<char> outCpuTempPtrs_;
};
#endif  // CATCH_EXAMPLES_OFFLINE_FCN8S_POST_PROCESS_FCN_OFF_POST_HPP_
