cmake_minimum_required(VERSION 2.8)
project(FatigueDrivingDetection)

add_compile_options(-std=c++11)

find_package(Caffe REQUIRED)
include_directories(${Caffe_DIR}/install/include/)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include/CaffeClassifier)
add_subdirectory(src/CaffeClassifier)

include_directories(include/3000fps include/3000fps/liblinear include/3000fps/liblinear/blas)
add_subdirectory(src/3000fps)

include_directories(include/fdd_lib include/forwarder_lib)
add_subdirectory(src/forwarder_lib)
option(WITH_SCREEN "the system has a screen" ON)
add_subdirectory(src/fdd_lib)

option(WITH_SOUND "the system can play sound" ON)
add_subdirectory(forwarder)

include_directories(include/dm)
add_subdirectory(src/dm)
add_subdirectory(DriverMonitor)

option(BUILD_TOOLS "build tools" ON)
if(BUILD_TOOLS)
    add_subdirectory(tools/CutOutFaceComponents)
    add_subdirectory(tools/LBF_demo)
    add_subdirectory(tools/TestFunc)
    add_subdirectory(tools/DrawRects)
    add_subdirectory(tools/MakeHeadposeDataset)
    add_subdirectory(tools/TestHeadposeModel)
    add_subdirectory(tools/GenerateGrayImages)
    add_subdirectory(tools/SplitImage)
endif(BUILD_TOOLS)


