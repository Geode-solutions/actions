#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::cutter_solid" for configuration "Release"
set_property(TARGET Geode-Common::cutter_solid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::cutter_solid PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_cutter_solid.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::geometry;Geode-Common::modifier_surface"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_cutter_solid.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::cutter_solid )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::cutter_solid "${_IMPORT_PREFIX}/lib/Geode-Common_cutter_solid.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_cutter_solid.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
