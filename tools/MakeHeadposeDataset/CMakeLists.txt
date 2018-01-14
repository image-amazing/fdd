aux_source_directory(. ALL_CPPS)
add_executable(make_head_pose_dataset ${ALL_CPPS})
target_link_libraries(make_head_pose_dataset ${OpenCV_LIBRARIES})
