find_library(3000FPS_LIB 3000fps)
find_library(LIBLINEAR_LIB liblinear PATHS ${CMAKE_INSTALL_PREFIX}/lib/liblinear)
find_library(BLAS_LIB blas PATHS ${CMAKE_INSTALL_PREFIX}/lib/liblinear/blas)
set(3000FPS_LIBS ${3000FPS_LIB} ${LIBLINEAR_LIB} ${BLAS_LIB})

