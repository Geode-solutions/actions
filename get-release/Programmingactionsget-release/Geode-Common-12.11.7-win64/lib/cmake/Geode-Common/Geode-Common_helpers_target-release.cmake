#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::helpers" for configuration "Release"
set_property(TARGET Geode-Common::helpers APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::helpers PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_helpers.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::mesh;OpenGeode::model;Geode-Common::geometry"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_helpers.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::helpers )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::helpers "${_IMPORT_PREFIX}/lib/Geode-Common_helpers.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_helpers.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
