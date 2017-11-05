project(LBF)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. LBF_CPPS)
include(${CMAKE_INSTALL_PREFIX}/share/3000fps.cmake)
add_executable(LBF ${LBF_CPPS})
target_link_libraries(LBF 3000fps)


