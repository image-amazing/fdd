aux_source_directory(. BLAS_CPPS)
add_library(blas SHARED ${BLAS_CPPS})
install(TARGETS blas LIBRARY DESTINATION lib)
