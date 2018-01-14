aux_source_directory(. ALL_CPPS)
add_executable(test_headpose_model ${ALL_CPPS})
target_link_libraries(test_headpose_model ${OpenCV_LIBRARIES} ${Caffe_LIBRARIES})
