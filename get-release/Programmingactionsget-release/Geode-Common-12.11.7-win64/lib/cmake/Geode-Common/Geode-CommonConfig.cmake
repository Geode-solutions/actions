#
# Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
#


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Geode-CommonConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

#################################################################################### 

include(CMakeFindDependencyMacro)
find_dependency(OpenGeode)

# Load information for each target
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_core_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_geometry_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_modifier_common_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_modifier_edged_curve_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_modifier_surface_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_modifier_solid_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_modifier_model_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_cutter_surface_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_cutter_solid_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_helpers_target.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Geode-Common_mesh_repair_target.cmake)
