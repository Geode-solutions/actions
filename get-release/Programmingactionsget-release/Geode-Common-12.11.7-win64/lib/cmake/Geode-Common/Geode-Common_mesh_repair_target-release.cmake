#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::mesh_repair" for configuration "Release"
set_property(TARGET Geode-Common::mesh_repair APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::mesh_repair PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_mesh_repair.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "OpenGeode::mesh"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_mesh_repair.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::mesh_repair )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::mesh_repair "${_IMPORT_PREFIX}/lib/Geode-Common_mesh_repair.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_mesh_repair.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
