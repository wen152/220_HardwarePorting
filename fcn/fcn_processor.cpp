#include "glog/logging.h"
//#ifdef USE_MLU
#include "cnrt.h" // NOLINT
#include "fcn_processor.hpp"
#include "fcn_runner.hpp"
#include "command_option.hpp"
#include "fcn_common_functions.hpp"

using std::pair;

template <class Dtype, template <class...> class Qtype>
void FcnProcessor<Dtype, Qtype>::initClassesInfo() {
    vector<string> classNameInfo = {"background", "aeroplane", "bicycle", "bird",
                                    "boat", "bottle", "bus", "car", "cat", "chair",
                                    "cow", "diningtable", "dog", "horse", "motobike",
                                    "person", "pottedplant", "sheep", "sofa", "train",
                                    "tvmonitor"};
    vector<vector<int>> rgbInfo = {{0, 0, 0}, {128, 0, 0}, {0, 128, 0},
                                   {128, 128, 0}, {0, 0, 128}, {128, 0, 128},
                                   {0, 128, 128}, {128, 128, 128}, {64, 0, 0},
                                   {192, 0, 0}, {64, 128, 0}, {192, 128, 0},
                                   {64, 0, 128}, {192, 0, 128}, {64, 128, 128},
                                   {192, 128, 128}, {0, 64, 0}, {128, 64, 0},
                                   {0, 192, 0}, {128, 192, 0}, {0, 64, 128}};
    for (int i = 0; i < 21; i++) {
        classInfoMap.insert(pair<int, string>(i, classNameInfo[i]));
    }
    for (int i = 0; i < 21; i++) {
        classRGBLabelVector.push_back(rgbInfo[i]);
    }
}

INSTANTIATE_ALL_CLASS(FcnProcessor);
//#endif
