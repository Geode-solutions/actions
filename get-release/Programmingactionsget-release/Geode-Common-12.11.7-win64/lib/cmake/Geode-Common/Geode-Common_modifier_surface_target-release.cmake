#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::modifier_surface" for configuration "Release"
set_property(TARGET Geode-Common::modifier_surface APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::modifier_surface PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_surface.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::mesh;Geode-Common::geometry"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_surface.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::modifier_surface )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::modifier_surface "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_surface.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_surface.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
