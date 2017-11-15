project(LBF_demo)
cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
aux_source_directory(. LBF_CPPS)
include(${CMAKE_INSTALL_PREFIX}/share/3000fps.cmake)
add_executable(${CMAKE_PROJECT_NAME} ${LBF_CPPS})
target_link_libraries(${CMAKE_PROJECT_NAME} 3000fps)


