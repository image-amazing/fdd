#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CaffeClassifier" for configuration ""
set_property(TARGET CaffeClassifier APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(CaffeClassifier PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "caffe"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libCaffeClassifier.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS CaffeClassifier )
list(APPEND _IMPORT_CHECK_FILES_FOR_CaffeClassifier "${_IMPORT_PREFIX}/lib/libCaffeClassifier.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
