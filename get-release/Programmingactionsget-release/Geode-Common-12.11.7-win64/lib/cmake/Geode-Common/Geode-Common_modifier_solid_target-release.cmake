#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::modifier_solid" for configuration "Release"
set_property(TARGET Geode-Common::modifier_solid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::modifier_solid PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_solid.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::mesh;Geode-Common::geometry"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_solid.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::modifier_solid )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::modifier_solid "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_solid.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_solid.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
