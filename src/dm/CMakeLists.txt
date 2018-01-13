#add dm_global library
add_library(dm_global SHARED dm_global.cpp)
target_link_libraries(dm_global config++)
install(TARGETS dm_global LIBRARY DESTINATION lib)
