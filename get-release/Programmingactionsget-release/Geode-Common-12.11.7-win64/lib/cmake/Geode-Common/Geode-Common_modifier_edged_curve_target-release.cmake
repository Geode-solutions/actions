#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::modifier_edged_curve" for configuration "Release"
set_property(TARGET Geode-Common::modifier_edged_curve APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::modifier_edged_curve PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_edged_curve.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::geometry;OpenGeode::mesh"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_edged_curve.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::modifier_edged_curve )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::modifier_edged_curve "${_IMPORT_PREFIX}/lib/Geode-Common_modifier_edged_curve.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_modifier_edged_curve.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
