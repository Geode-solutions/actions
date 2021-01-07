#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::geometry" for configuration "Release"
set_property(TARGET Geode-Common::geometry APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::geometry PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_geometry.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::geometry"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_geometry.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::geometry )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::geometry "${_IMPORT_PREFIX}/lib/Geode-Common_geometry.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_geometry.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
