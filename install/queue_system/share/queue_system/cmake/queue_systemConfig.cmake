# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_queue_system_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED queue_system_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(queue_system_FOUND FALSE)
  elseif(NOT queue_system_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(queue_system_FOUND FALSE)
  endif()
  return()
endif()
set(_queue_system_CONFIG_INCLUDED TRUE)

# output package information
if(NOT queue_system_FIND_QUIETLY)
  message(STATUS "Found queue_system: 0.0.0 (${queue_system_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'queue_system' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${queue_system_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(queue_system_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${queue_system_DIR}/${_extra}")
endforeach()
