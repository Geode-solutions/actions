#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::cutter_surface" for configuration "Release"
set_property(TARGET Geode-Common::cutter_surface APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::cutter_surface PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_cutter_surface.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::geometry"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_cutter_surface.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::cutter_surface )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::cutter_surface "${_IMPORT_PREFIX}/lib/Geode-Common_cutter_surface.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_cutter_surface.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
