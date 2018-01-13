aux_source_directory(. SRC_LIST)
add_library(MessageQueue  SHARED MessageQueue.cpp Exception.cpp)
install(TARGETS MessageQueue LIBRARY DESTINATION lib)
