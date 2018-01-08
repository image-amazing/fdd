aux_source_directory(. BLAS_CPPS)
add_library(blas ${BLAS_CPPS})
install(TARGETS blas EXPORT 3000fps DESTINATION lib/liblinear/blas)
install(FILES blas.h blasp.h DESTINATION include/liblinear/blas)
