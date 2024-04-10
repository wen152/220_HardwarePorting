#ifndef CATCH_EXAMPLES_OFFLINE_SSD_POST_PROCESS_SSD_PROCESSOR_HPP_
#define CATCH_EXAMPLES_OFFLINE_SSD_POST_PROCESS_SSD_PROCESSOR_HPP_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "glog/logging.h"
#include "post_processor.hpp"
#include "threadPool.hpp"

using std::map;
using std::max;
using std::min;
using std::queue;
using std::stringstream;
using std::vector;
using std::string;

class Mask{
  public:
    Mask(vector<float> bbox_, cv::Mat mask_, cv::Size ori_size) { 
      for(float b : bbox_){
        bbox.push_back(b);
      }
      mask = mask_.clone();
      cv::Mat tmp1, tmp2;
      expand_mask(1);
      expand_bbox();
      // resize mask
      int w = std::max(int(bbox_expand[2] - bbox_expand[0] + 1), 1);
      int h = std::max(int(bbox_expand[3] - bbox_expand[1] + 1), 1);
      cv::resize(mask_expand, tmp2, cv::Size(w, h));
      cv::threshold(tmp2, mask, 0.5, 255, cv::THRESH_BINARY);
      mask_to_ori_img(ori_size);
    }

    Mask(const Mask& obj){
      for(int i = 0; i < obj.bbox.size(); i++){
        bbox.push_back(obj.bbox[i]);
        bbox_expand.push_back(obj.bbox_expand[i]);
      }
      mask = obj.mask.clone();
      mask_expand = obj.mask_expand.clone();
      scale = obj.scale;
    }
    
    ~Mask(){}
    vector<float> bbox;
    cv::Mat mask;
    cv::Mat mask_expand;
    cv::Mat mask_ori;
    vector<int> bbox_expand;
    float scale = 1.0;
    float score = 0.0;
    int label = 0;

  private:
    void expand_mask(int padding){
      int mask_h = mask.cols;
      int mask_w = mask.rows;
      int pad2 = 2 * padding;
      scale = float(mask_h + pad2) / float(mask_h);
      mask_expand = cv::Mat(mask_h + pad2, mask_h + pad2, CV_32FC1, cv::Scalar(0.0));
      mask.copyTo(mask_expand(cv::Rect(padding, padding, mask_w, mask_h)));
    }

    void expand_bbox(){
      float w_half = (bbox[2] - bbox[0]) * 0.5;
      float h_half = (bbox[3] - bbox[1]) * 0.5;
      float x_c = (bbox[2] + bbox[0]) * 0.5;
      float y_c = (bbox[3] + bbox[1]) * 0.5;

      w_half *= scale;
      h_half *= scale;

      bbox_expand.resize(bbox.size());
      bbox_expand[0] = int(x_c - w_half);
      bbox_expand[2] = int(x_c + w_half);
      bbox_expand[1] = int(y_c - h_half);
      bbox_expand[3] = int(y_c + h_half);
    }

    void mask_to_ori_img(cv::Size ori_size){
      int im_w = ori_size.width;
      int im_h = ori_size.height;
      mask_ori = cv::Mat(ori_size, CV_8UC1, cv::Scalar(0));
      int x_0 = std::min(std::max(bbox_expand[0], 0), im_w);
      int x_1 = std::max(std::min(bbox_expand[2] + 1, im_w), 0);
      int y_0 = std::min(std::max(bbox_expand[1], 0), im_h);
      int y_1 = std::max(std::min(bbox_expand[3] + 1, im_h), 0);
      cv::Mat temp = mask(cv::Rect(cv::Point(x_0 - bbox_expand[0], y_0 - bbox_expand[1]), 
                    cv::Point(x_1 - bbox_expand[0], y_1 - bbox_expand[1])));
      temp.copyTo(mask_ori(cv::Rect(cv::Point(x_0, y_0), cv::Point(x_1, y_1))));
      // printf("{%d,%d,%d,%d}\n", x_0, y_0, x_1, y_1);
    }
};

class MaskrcnnProcessor : public PostProcessor {
  public:
  MaskrcnnProcessor() {}
  virtual ~MaskrcnnProcessor() {}
  virtual void runParallel();
  void WriteVisualizeBBox_offline(const vector<cv::Mat>& images,
      const vector<vector<vector<float>>> detections,
      const vector<string>& labelToDisplayName, const vector<string>& imageNames,
      int input_dim, const int from, const int to);
  void readLabels(std::vector<string>& label_name);

  protected:
  std::vector<string> label_name;

  private:
    std::vector<std::vector<Mask>> getResults(vector<std::pair<string, cv::Mat>> origin_img, 
                                              const vector<string>& labelToDisplayName);
    void getImages(std::vector<cv::Mat>*imgs,
                   std::vector<std::string>*img_names,
                   std::vector<std::pair<std::string, cv::Mat>> origin_img);

    Mask map_box_to_ori_img(float* box, float* mask, cv::Size ori_shape);

    template<typename T>
    inline std::shared_ptr<T> MakeArray(int size) {
      return std::shared_ptr<T>( new T[size],
        [size](T *p ) {
          for (int i = 0; i < size; i++)
            delete[] p[i];
          delete[] p;
        });
    }

    void findNameId(const string img_name, string &name, string &id){
      name = img_name;
      int positionMap = name.rfind("/");
      if (positionMap > 0 && positionMap < name.size()) {
        name = name.substr(positionMap + 1);
      }
      positionMap = name.find(".");
      if (positionMap > 0 && positionMap < name.size()) {
        name = name.substr(0, positionMap);
      }
      int i = 0;
      char c = name[i];
      while(c == '0' && i < name.size()){
        i++;
        c = name[i];
      }
      id = name.substr(i, name.size() - i);
      // std::cout << "image id:" << id << std::endl;
    }
  
   std::shared_ptr<float*> outCpuPtrs_;
   std::shared_ptr<float*> outTrans_;
   std::shared_ptr<char*> outCpuTempPtrs_;
};

#endif  // CATCH_EXAMPLES_OFFLINE_SSD_POST_PROCESS_SSD_PROCESSOR_HPP_
