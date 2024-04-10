#set minimum camke version
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
add_compile_options(-std=c++11)
#project name and language
project(main LANGUAGES CXX)
#set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -DUSE_MLU -DUSE_OPENCV")
set(CMAKE_CXX_FLAGS  "-std=c++11 -pthread")
#SET(CMAKE_CXX_COMPILER /home/user/mlu资料/SDK_1.6.0_demo_soc_source_code/MLU220-soc/aarch64/gcc-linaro-6.2.1-2016.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

#指定编译类型，debug或者为release
#debug会生成相关调试信息，可以使用GDB进行
#release不会生成调试信息，当无法进行调试时查看此处是否设置为debug.
set(CMAKE_BUILD_TYPE Debug)
set(TRACK ${PROJECT_SOURCE_DIR}/track)
set(ISARIMAGING ${PROJECT_SOURCE_DIR}/ISARimaging)
set(MASKRCNN ${PROJECT_SOURCE_DIR}/mask_rcnn)
set(FCN ${PROJECT_SOURCE_DIR}/fcn)
set(FUSION_MEASURE ${PROJECT_SOURCE_DIR}/FusionMeasure)
file(GLOB_RECURSE track ${CMAKE_CURRENT_SOURCE_DIR}/track/*.cpp)
file(GLOB_RECURSE isar ${CMAKE_CURRENT_SOURCE_DIR}/ISARimaging/*.cpp)
file(GLOB_RECURSE mask ${CMAKE_CURRENT_SOURCE_DIR}/mask_rcnn/*.cpp)
file(GLOB_RECURSE fcn ${CMAKE_CURRENT_SOURCE_DIR}/fcn/*.cpp)
file(GLOB_RECURSE measure ${CMAKE_CURRENT_SOURCE_DIR}/FusionMeasure/*.cpp)

find_package(OpenMP REQUIRED)
#include_directories(/home/user/SDK_demo/include/opencv/include)
#include_directories(/home/user/SDK_demo/include/gflags/include)
include_directories(${TRACK}/include)
include_directories(${ISARIMAGING}/include)
include_directories(/home/user/SDK_demo/include/neuware/include)
include_directories(${MASKRCNN}/include)
include_directories(${FCN}/include)
include_directories(${FUSION_MEASURE}/include)
include_directories("./common/include")
#include_directories(/home/user/SDK_demo/include/glog/include)
#include_directories(/home/user/include)
link_directories(/home/user/lib_needed/opencv-3.4.4/lib)
link_directories(/home/user/lib_needed/fftw/lib)
link_directories(/usr/local/neuware/lib64)
#x86
#link_directories(/home/user/opencv/aarch_64_install/lib)
#link_directories(/home/user/lib64)

add_executable(${PROJECT_NAME} 
main.cpp	
${track}
${isar}
${mask}
${fcn}
${measure}
./common/gflags_common.cpp
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    pthread
    protobuf
    fftw3
    gflags
    gomp
    glog
    opencv_core
    opencv_imgproc
    opencv_highgui
    opencv_imgcodecs
    cnrt
    boost_system
    boost_thread
    boost_atomic
)
