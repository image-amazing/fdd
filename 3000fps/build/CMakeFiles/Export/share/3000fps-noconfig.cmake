#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "blas" for configuration ""
set_property(TARGET blas APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(blas PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liblinear/blas/libblas.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS blas )
list(APPEND _IMPORT_CHECK_FILES_FOR_blas "${_IMPORT_PREFIX}/lib/liblinear/blas/libblas.a" )

# Import target "liblinear" for configuration ""
set_property(TARGET liblinear APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(liblinear PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "blas"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liblinear/libliblinear.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS liblinear )
list(APPEND _IMPORT_CHECK_FILES_FOR_liblinear "${_IMPORT_PREFIX}/lib/liblinear/libliblinear.a" )

# Import target "3000fps" for configuration ""
set_property(TARGET 3000fps APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(3000fps PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "liblinear;opencv_calib3d;opencv_core;opencv_features2d;opencv_flann;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_ml;opencv_objdetect;opencv_photo;opencv_shape;opencv_stitching;opencv_superres;opencv_video;opencv_videoio;opencv_videostab;opencv_viz"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/lib3000fps.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS 3000fps )
list(APPEND _IMPORT_CHECK_FILES_FOR_3000fps "${_IMPORT_PREFIX}/lib/lib3000fps.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
