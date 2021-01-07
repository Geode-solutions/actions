#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Geode-Common::core" for configuration "Release"
set_property(TARGET Geode-Common::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Geode-Common::core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Geode-Common_core.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Geode-Common_core.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Geode-Common::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_Geode-Common::core "${_IMPORT_PREFIX}/lib/Geode-Common_core.lib" "${_IMPORT_PREFIX}/bin/Geode-Common_core.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
