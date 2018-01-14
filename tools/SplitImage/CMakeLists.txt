aux_source_directory(. SRC_LIST)
add_executable(SplitImage ${SRC_LIST})
target_link_libraries(SplitImage ${OpenCV_LIBRARIES} dm_global)
