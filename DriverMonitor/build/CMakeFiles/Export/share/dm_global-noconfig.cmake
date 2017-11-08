#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dm_global" for configuration ""
set_property(TARGET dm_global APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(dm_global PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "config++"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libdm_global.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS dm_global )
list(APPEND _IMPORT_CHECK_FILES_FOR_dm_global "${_IMPORT_PREFIX}/lib/libdm_global.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
