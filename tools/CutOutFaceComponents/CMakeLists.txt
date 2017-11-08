project(CutOutFaceComponents)
cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
find_package(Caffe REQUIRED)
include_directories(${Caffe_DIR}/install/include)
include(${CMAKE_INSTALL_PREFIX}/share/3000fps.cmake)
include(${CMAKE_INSTALL_PREFIX}/share/CaffeClassifier.cmake)
include(${CMAKE_INSTALL_PREFIX}/share/libfdd.cmake)
include(${CMAKE_INSTALL_PREFIX}/share/dm_global.cmake)
aux_source_directory(. COFC_CPPS)
add_executable(CutOutFaceComponents ${COFC_CPPS})
target_link_libraries(CutOutFaceComponents libfdd dm_global)


