
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "data_provider.hpp"
#include "pipeline.hpp"
#include "runner.hpp"

#include "command_option.hpp"
#include "common_functions.hpp"
#include <fstream>

bool DataProvider::imageIsEmpty() {
  if (this->imageList.empty()) {
    return true;
  }
  return false;
}

void DataProvider::readOneBatch(std::vector<std::pair<std::string, cv::Mat>>& ImageAndName) {
  std::string file_id , file;
  cv::Mat prev_image;
  int image_read = 0;

  while (image_read < this->offlineDescripter()->inN()) {
    if (!this->imageList.empty()) {
      file = file_id = this->imageList.front();
      this->imageList.pop();
      if (file.find(" ") != std::string::npos)
        file = file.substr(0, file.find(" "));
      cv::Mat img;
      if (FLAGS_mask_yuv) {
        img = convertYuv2Mat(file, inGeometry_);
      } else {
        img = cv::imread(file, -1);
      }
      if (img.data) {
        ++image_read;
        prev_image = img;
        ImageAndName.push_back(std::pair<std::string, cv::Mat>(file_id, img));

      } else {
        LOG(INFO) << "failed to read " << file;
      }
    } else {
      if (image_read) {
        cv::Mat img;
        ++image_read;
        prev_image.copyTo(img);
        ImageAndName.push_back(std::pair<std::string, cv::Mat>("null", img));
      } else {
        // if the que is empty and no file has been read, no more runs
        LOG(ERROR) << "No image is loaded, please check your image list";
      }
    }
  }
}

void DataProvider::WrapInputLayer(std::vector<std::vector<cv::Mat> >* wrappedImages,
                                  float* inputData) {
  //  Parameter images is a vector [ ----   ] <-- images[in_n]
  //                                |
  //                                |-> [ --- ] <-- channels[3]
  // This method creates Mat objects, and places them at the
  // right offset of input stream
  int width = this->offlineDescripter()->inW();
  int height = this->offlineDescripter()->inH();
  int channels = FLAGS_mask_yuv ? 1 : this->offlineDescripter()->inC();
  for (int i = 0; i < this->offlineDescripter()->inN(); ++i) {
    wrappedImages->push_back(std::vector<cv::Mat> ());
    for (int j = 0; j < channels; ++j) {
      if (FLAGS_mask_yuv) {
        cv::Mat channel(height, width, CV_8UC1, reinterpret_cast<char*>(inputData));
        (*wrappedImages)[i].push_back(channel);
        inputData += width * height / 4;
      } else {
        cv::Mat channel(height, width, CV_32FC1, inputData);
        (*wrappedImages)[i].push_back(channel);
        inputData += width * height;
      }
    }
  }
}

void DataProvider::convertColor(const cv::Mat& sourceImage, cv::Mat& sample) {
  // convert sourceImage colors, where inChannel is cambricon model required input
  // channel. There might be some cases that channel of input image inconsistent with
  // input channel of cambricon model, e.g. firstconv.
  int inChannel = this->offlineDescripter()->inC();
  if (sourceImage.channels() == 3 && inChannel == 1)
    cv::cvtColor(sourceImage, sample, cv::COLOR_BGR2GRAY);
  else if (sourceImage.channels() == 4 && inChannel == 1)
    cv::cvtColor(sourceImage, sample, cv::COLOR_BGRA2GRAY);
  else if (sourceImage.channels() == 4 && inChannel == 3)
    cv::cvtColor(sourceImage, sample, cv::COLOR_BGRA2BGR);
  else if (sourceImage.channels() == 1 && inChannel == 3)
    cv::cvtColor(sourceImage, sample, cv::COLOR_GRAY2BGR);
  else if (sourceImage.channels() == 3 && inChannel == 4)
    cv::cvtColor(sourceImage, sample, cv::COLOR_BGR2RGBA);
  else if (sourceImage.channels() == 1 && inChannel == 4)
    cv::cvtColor(sourceImage, sample, cv::COLOR_GRAY2RGBA);
  else
    sample = sourceImage;
}

void DataProvider::resizeMat(const cv::Mat& sample, cv::Mat& sample_resized) {
    if (sample.size() == inGeometry_) {
      sample_resized = sample;
      return;
    }
    cv::resize(sample, sample_resized, inGeometry_);
}

void DataProvider::convertFloat(const cv::Mat& sample_resized, cv::Mat& sample_float) {
  int inChannel = this->offlineDescripter()->inC();
  if (inChannel == 3) {
    sample_resized.convertTo(sample_float, CV_32FC3);
  } else if (inChannel == 1) {
    sample_resized.convertTo(sample_float, CV_32FC1);
  } else if (inChannel == 4) {
    sample_resized.convertTo(sample_float, CV_32FC4);
  }
}

void DataProvider::normalizeMat(const cv::Mat& sample_float, cv::Mat& sample_normalized) {
  if (!meanValue_.empty()) {
    cv::subtract(sample_float, mean_, sample_normalized);
    if (FLAGS_mask_scale != 1)
      sample_normalized *= FLAGS_mask_scale;
  } else {
    sample_normalized = sample_float;
  }
}

void DataProvider::Preprocess(
    const std::vector<std::pair<std::string, cv::Mat>>& sourceImages,
    std::vector<std::vector<cv::Mat> >* destImages) {
  /* Convert the input image to the input image format of the network. */
  CHECK(sourceImages.size() == destImages->size())
    << "Size of sourceImages and destImages doesn't match";
  for (int i = 0; i < sourceImages.size(); ++i) {
    auto& sourceImage = sourceImages[i].second;
    if (FLAGS_mask_yuv) {
      cv::Mat sample_yuv;
      sourceImage.convertTo(sample_yuv, CV_8UC1);
      cv::split(sample_yuv, (*destImages)[i]);
      continue;
    }
    cv::Mat sample;
    convertColor(sourceImage, sample);
    cv::Mat sample_resized;
    resizeMat(sample, sample_resized);
    cv::Mat sample_float;
    convertFloat(sample_resized, sample_float);
    cv::Mat sample_normalized;
    normalizeMat(sample_float, sample_normalized);
    /* This operation will write the separate BGR planes directly to the
     * input layer of the network because it is wrapped by the cv::Mat
     * objects in input_channels. */
    cv::split(sample_normalized, (*destImages)[i]);
  }
}

void DataProvider::copyin(std::shared_ptr<BoxingData> input_boxing_data) {
  int dim_order[4] = {0, 2, 3, 1};
  auto input_shape = this->offlineDescripter()->getInputShape();
  // XXX: shape should be unsigned int, but cnrt interface only accept int* pointer.
  // static_cast is needed to do the tricky
  int dim_shape[4] = {static_cast<int>(input_shape[0]), /*n*/
                      static_cast<int>(input_shape[1]), /*c*/
                      static_cast<int>(input_shape[2]), /*h*/
                      static_cast<int>(input_shape[3])}; /*w*/
  int inputDimValue[4] = {static_cast<int>(input_shape[0]), /*n*/
                          static_cast<int>(input_shape[2]), /*h*/
                          static_cast<int>(input_shape[3]), /*w*/
                          static_cast<int>(input_shape[1])}; /*c*/
  int temp_input_count = this->offlineDescripter()->inCount();
  int inputDimStride[4] = {0, 0, 0, 1};

  int inputNum = this->offlineDescripter()->inputNum();
  cnrtDataType_t* input_data_type = this->offlineDescripter()->getInputDataType();

  // retrieve data from queue
  auto mluData = input_boxing_data->getBuf();
  auto queue = input_boxing_data->getQueue();

  auto cpuCastData = reinterpret_cast<void**>(cpuCastData_);

  for (int i = 0; i < inputNum; i++) {
    void* temp_ptr = nullptr;
    cnrtTransDataOrder(cpuData_[i],
                       CNRT_FLOAT32,
                       cpuTrans_[i],
                       4,
                       dim_shape,
                       dim_order);
    temp_ptr = cpuTrans_[i];

    if (input_data_type[i] != CNRT_FLOAT32) {
      cnrtCastDataType(cpuTrans_[i],
                       CNRT_FLOAT32,
                       cpuCastData[i],
                       input_data_type[i],
                       temp_input_count,
                       nullptr);
      temp_ptr = cpuCastData[i];
    }
    std::cout << "input format:" << FLAGS_mask_input_format << std::endl;

    // FLAGS_input_format indicates the channel format defined in CAMBRICON MODEL.
    if (i == 0 && this->offlineDescripter()->isFirstConv()) {
      if (FLAGS_mask_input_format == 0) {
        // input image is rgb(bgr) format
        // input_format=0 represents weight is in rgb(bgr) order
        cnrtAddDataStride(cpuCastData[0], CNRT_UINT8, cpuStrideData_.get(), 4,
                          inputDimValue, inputDimStride);
      } else if (FLAGS_mask_input_format == 1 || FLAGS_mask_input_format == 3) {
        // input data is in rgba(bgra) format
        // input_format=1 represents weight is in argb(abgr) order
        // input_data uses rgba and weight uses argb. To make format unified,
        // a circle shift is required.
        memcpy(cpuStrideData_.get() + 1, cpuCastData[0], temp_input_count - 1);
        cpuStrideData_.get()[0] = 0;
      } else if (FLAGS_mask_input_format == 2) {
        // input data is in bgra(rgba) format
        // input_format=2 represents weight is in bgra(rgba) order
        memcpy(cpuStrideData_.get(), cpuCastData[0], temp_input_count);
      }
      temp_ptr = cpuStrideData_.get();
    }
    if (FLAGS_mask_async_copy) {
      cnrtMemcpyAsync(mluData.get()[i],
                      temp_ptr,
                      this->offlineDescripter()->inputSizeS[i],
                      *queue,
                      CNRT_MEM_TRANS_DIR_HOST2DEV);
    } else {
      cnrtMemcpy(mluData.get()[i],
                 temp_ptr,
                 this->offlineDescripter()->inputSizeS[i],
                 CNRT_MEM_TRANS_DIR_HOST2DEV);
    }
  }
}

void DataProvider::runParallel() {
  setDeviceId(deviceId_);
  std::cout << "deviceId_:" << deviceId_ << std::endl;

  prepareBuffer();
  Pipeline::waitForNotification();

  while (this->imageList.size()) {
    Timer preprocessor;
    Timer readimage;
    std::vector<std::pair<std::string, cv::Mat>> imageNameVec;
    this->readOneBatch(imageNameVec);
    readimage.log("read image by opencv ...");
    std::vector<std::vector<cv::Mat> > preprocessedImages;
    Timer prepareInput;
    this->WrapInputLayer(&preprocessedImages, reinterpret_cast<float*>(cpuData_[0]));
    this->Preprocess(imageNameVec, &preprocessedImages);
    prepareInput.log("prepare input data ...");

    auto input_boxing_data = this->offlineDescripter()->popFreeInputBoxingData(deviceId_);
    auto time_stamp = input_boxing_data->getStamp();
    Timer copyin;
    TimePoint t1 = std::chrono::high_resolution_clock::now();
    this->copyin(input_boxing_data);
    copyin.log("copyin time ...");
    TimePoint t2 = std::chrono::high_resolution_clock::now();
    time_stamp->in_start = t1;
    time_stamp->in_end = t2;

    // set boxing data
    input_boxing_data->setImageAndName(imageNameVec);
    this->offlineDescripter()->pushValidInputBoxingData(input_boxing_data, deviceId_);
    preprocessor.log("preprocessor time ...");
  }
  LOG(INFO) << "DataProvider: no data ...";
  // tell runner there is no more images
}

void DataProvider::SetMean() {
  if (!this->meanValue_.empty())
    SetMeanValue();
}

void DataProvider::SetMeanValue() {
  if (FLAGS_mask_yuv) return;
  std::stringstream ss(this->meanValue_);
  std::vector<float> values;
  std::string item;
  while (getline(ss, item, ',')) {
    float value = std::atof(item.c_str());
    values.push_back(value);
  }
  int inChannel = this->offlineDescripter()->inC();
  CHECK(values.size() == 1 || values.size() == inChannel) <<
    "Specify either one mean value or as many as channels: " << inChannel;
  std::vector<cv::Mat> channels;
  for (int i = 0; i < inChannel; ++i) {
    /* Extract an individual channel. */
    cv::Mat channel(this->inGeometry_.height, this->inGeometry_.width, CV_32FC1,
                    cv::Scalar(values[i]));
    channels.push_back(channel);
  }
  cv::merge(channels, this->mean_);
}

void DataProvider::prepareBuffer() {
  // Allocate buffers used in dataprovder to avoid malloc repeatly in for loop
  // TODO: consider whether it is proper to do shape intialization and set mean in prepareBuffer
  int inHeight = this->offlineDescripter()->inH();
  int inWidth = this->offlineDescripter()->inW();
  this->inGeometry_ = cv::Size(inWidth, inHeight);
  this->SetMean();
  // TODO: use shared_ptr instead of raw pointer and refactor cpudata
  int inputNum = this->offlineDescripter()->inputNum();
  for (int i = 0; i < inputNum; ++i) {
    int input_count = this->offlineDescripter()->inCount(i);
    cpuData_ = new(void*);
    cpuData_[i] = new float[input_count];
    cpuTrans_ = new(void*);
    cpuTrans_[i] = new float[input_count];

    cpuCastData_ = new(void*);
    if (i == 0 && this->offlineDescripter()->isFirstConv()) {
      // The differences between input_count and inputSizeS[i] is that:
      // input_count = n * c * h * w
      // inputSizeS[i] = input_count * sizeof(data_type)
      // cpuData_ is allocated by new float[input_count]
      // cpuCastData_ is the casted data which size depends on the data type
      // Here's the tricky for firstconv. The actual input channel for firstconv
      // is 3 with data type uint8, while channel in cambricon is 4. That is,
      // in this senario, input_count = 3nhw, while inputSizeS[0]=4nhw.
      cpuCastData_[i] = new char[input_count];
      cpuStrideData_ = std::shared_ptr<uint8_t>(
        new uint8_t[this->offlineDescripter()->inputSizeS[0]],
        std::default_delete<uint8_t[]>());
    } else {
      cpuCastData_[i] = new char[this->offlineDescripter()->inputSizeS[i]];
    }
  }
}

