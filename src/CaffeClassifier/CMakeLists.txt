aux_source_directory(. CAFFECLASSIFIER_CPPS)
add_library(CaffeClassifier SHARED ${CAFFECLASSIFIER_CPPS})
target_link_libraries(CaffeClassifier ${Caffe_LIBRARIES})
install(TARGETS CaffeClassifier LIBRARY DESTINATION lib)
