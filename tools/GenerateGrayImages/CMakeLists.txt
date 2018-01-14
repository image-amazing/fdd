aux_source_directory(. SRC_LIST)
add_executable(GenerateGrayImages ${SRC_LIST})
target_link_libraries(GenerateGrayImages ${OpenCV_LIBRARIES} dm_global)

