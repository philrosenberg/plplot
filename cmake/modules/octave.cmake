# cmake/modules/octave.cmake
#
# Copyright (C) 2006-2010  Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining octave bindings configuration options

# Options to enable Octave bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_octave "Enable Octave bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_octave "Enable Octave bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_octave AND NOT CMAKE_CXX_COMPILER_WORKS)
  message(STATUS "WARNING: "
    "Octave requires working C++ compiler to build.  Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT CMAKE_CXX_COMPILER_WORKS)

if(ENABLE_octave AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling Octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT SWIG_FOUND)

if(ENABLE_octave AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Octave requires shared libraries. Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT BUILD_SHARED_LIBS)

if(ENABLE_octave AND NOT PERL_FOUND)
  message(STATUS "WARNING: "
    "The octave build requires perl. Disabling octave bindings")
  set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
endif(ENABLE_octave AND NOT PERL_FOUND)

if(ENABLE_octave)
  find_program(OCTAVE octave)
  if(OCTAVE)
    message(STATUS "OCTAVE = ${OCTAVE}")
  else(OCTAVE)
    message(STATUS "WARNING: "
    "octave not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(OCTAVE)
endif(ENABLE_octave)

if(ENABLE_octave)
  find_program(MKOCTFILE mkoctfile)
  if(MKOCTFILE)
    message(STATUS "MKOCTFILE = ${MKOCTFILE}")
  else(MKOCTFILE)
    message(STATUS "WARNING: "
    "mkoctfile not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(MKOCTFILE)
endif(ENABLE_octave)

if(ENABLE_octave)
  find_program(OCTAVE_CONFIG octave-config)
  if(OCTAVE_CONFIG)
    message(STATUS "OCTAVE_CONFIG = ${OCTAVE_CONFIG}")
  else(OCTAVE_CONFIG)
    message(STATUS "WARNING: "
    "octave-config not found. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(OCTAVE_CONFIG)
endif(ENABLE_octave)

if(ENABLE_octave)
  #OCTAVE_VERSION is the (dotted triplet) octave version.
  execute_process(
    COMMAND ${OCTAVE_CONFIG} -p VERSION
    OUTPUT_VARIABLE OCTAVE_VERSION
    ERROR_VARIABLE OCTAVE_ERROR
    RESULT_VARIABLE return_code
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(return_code)
    message(STATUS "OCTAVE_ERROR = ${OCTAVE_ERROR}")
    message(STATUS "WARNING: "
    "${OCTAVE_CONFIG} -p VERSION generates an error (non-zero return code).  Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(return_code)
endif(ENABLE_octave)

if(ENABLE_octave)
  message(STATUS "OCTAVE_VERSION = ${OCTAVE_VERSION}")
  # Logic that depends on octave version
  transform_version(NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION "3.2.0")
  transform_version(NUMERICAL_OCTAVE_VERSION "${OCTAVE_VERSION}")
  if(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
    message(STATUS "WARNING: "
    "plplot require octave version 3.2 or greater. Disabling octave bindings")
    set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
  endif(
   NUMERICAL_OCTAVE_VERSION
   LESS
   "${NUMERICAL_OCTAVE_TESTING_MINIMUM_VERSION}"
   )
endif(ENABLE_octave)

if(ENABLE_octave)
  # The following if block is required to keep from repeated pre-pending
  # of OCTAVE_INCLUDE_PATH_TRIMMED onto OCTAVE_INCLUDE_PATH which would
  # create 2^n components to OCTAVE_INCLUDE_PATH for n cmake calls.
  # Note, all externally used results (OCTAVE_INCLUDE_PATH, OCTAVE_LIBRARIES,
  # OCTINTERP_LIBRARIES, and ENABLE_octave) are cached so skipping everything
  # if OCTAVE_INCLUDE_PATH is defined from the previous cmake run should be
  # fine.
  if(NOT DEFINED OCTAVE_INCLUDE_PATH)
    execute_process(
    COMMAND ${OCTAVE_CONFIG} -p OCTINCLUDEDIR
    OUTPUT_VARIABLE OCTAVE_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    find_path(
    OCTAVE_INCLUDE_PATH
    oct.h
    PATHS ${OCTAVE_INCLUDE_DIR}
    PATH_SUFFIXES octave
    )

    execute_process(
    COMMAND ${OCTAVE_CONFIG} -p OCTLIBDIR
    OUTPUT_VARIABLE OCTAVE_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    find_library(
    OCTAVE_LIBRARIES
    octave
    PATHS ${OCTAVE_LIB_DIR}
    )
    
    find_library(
    OCTINTERP_LIBRARIES
    octinterp
    PATHS ${OCTAVE_LIB_DIR}
    )

    if(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES)
      # If first octave include path has trailing /octave, then must have
      # second include path without that trailing /octave.
      string(REGEX REPLACE "/octave$" ""
      OCTAVE_INCLUDE_PATH_TRIMMED
      ${OCTAVE_INCLUDE_PATH}
      )
      if(NOT OCTAVE_INCLUDE_PATH_TRIMMED STREQUAL "${OCTAVE_INCLUDE_PATH}")
        set(OCTAVE_INCLUDE_PATH 
	${OCTAVE_INCLUDE_PATH_TRIMMED} ${OCTAVE_INCLUDE_PATH}
	CACHE INTERNAL ""
	)
      endif(NOT OCTAVE_INCLUDE_PATH_TRIMMED STREQUAL "${OCTAVE_INCLUDE_PATH}")
      # Octave has a huge number of dependencies and therefore an
      # impossible-to-untangle set of header #includes that depend on
      # other packages headers.  And there is no information from
      # mkoctfile or the octave octave_config_info command about where
      # those header directories are located.  But from experiments
      # with both the Linux and Windows binary versions of octave, it
      # appears that hdf5.h is one external header that is necessary,
      # and it is never part of the octave-${OCTAVE_VERSION}/octave
      # hierarchy so that PATH_SUFFIXES signature is dropped.
      find_path(
	OCTAVE_INCLUDE_PATH_EXTERNAL
	hdf5.h
	)
      if(OCTAVE_INCLUDE_PATH_EXTERNAL)
        set(OCTAVE_INCLUDE_PATH 
	${OCTAVE_INCLUDE_PATH_EXTERNAL} ${OCTAVE_INCLUDE_PATH}
	CACHE INTERNAL ""
	)
      else(OCTAVE_INCLUDE_PATH_EXTERNAL)
	message(STATUS "WARNING: "
	  "Required external octave header, hdf5.h, not found. Disabling octave bindings")
	set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
      endif(OCTAVE_INCLUDE_PATH_EXTERNAL)
    else(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES)
      message(STATUS "WARNING: "
      "octave headers and/or library not found. Disabling octave bindings")
      set(ENABLE_octave OFF CACHE BOOL "Enable Octave bindings" FORCE)
    endif(OCTAVE_INCLUDE_PATH AND OCTAVE_LIBRARIES AND OCTINTERP_LIBRARIES) 
  endif(NOT DEFINED OCTAVE_INCLUDE_PATH)
  message(STATUS "OCTAVE_LIBRARIES = ${OCTAVE_LIBRARIES}")
  message(STATUS "OCTINTERP_LIBRARIES = ${OCTINTERP_LIBRARIES}")
  message(STATUS "OCTAVE_INCLUDE_PATH = ${OCTAVE_INCLUDE_PATH}")
endif(ENABLE_octave)

if(ENABLE_octave)
  # PLPLOT_OCTAVE_DIR is the directory for installation of the PLplot_Octave
  # specific m files
  set(PLPLOT_OCTAVE_DIR ${CMAKE_INSTALL_DATADIR}/plplot_octave)
  message(STATUS "PLPLOT_OCTAVE_DIR = ${PLPLOT_OCTAVE_DIR}")
  
  # OCTAVE_PREFIX is the prefix where octave was installed.
  # N.B. this file method is really clunky, but we are forced to use
  # this method because as far as I know there is no method
  # of invoking octave scripts from the octave command line other than
  # with a file.
  file(WRITE ${CMAKE_BINARY_DIR}/octave_command
  "printf(octave_config_info(\"prefix\"));"
  )
  execute_process(
  COMMAND ${OCTAVE} -q -f octave_command
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  OUTPUT_VARIABLE OCTAVE_PREFIX
  )
  #message(STATUS "OCTAVE_PREFIX = ${OCTAVE_PREFIX}")
  file(TO_CMAKE_PATH ${OCTAVE_PREFIX} OCTAVE_PREFIX)
  #message(STATUS "(CMake) OCTAVE_PREFIX = ${OCTAVE_PREFIX}")
  
  # octave-2.1 (or higher) logic.
  #_OCTAVE_M_DIR
  file(WRITE ${CMAKE_BINARY_DIR}/octave_command
  "printf(octave_config_info(\"localfcnfiledir\"));"
  )
  execute_process(
  COMMAND ${OCTAVE} -q -f octave_command
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  OUTPUT_VARIABLE _OCTAVE_M_DIR
  )
  #message(STATUS "_OCTAVE_M_DIR = ${_OCTAVE_M_DIR}")
  file(TO_CMAKE_PATH ${_OCTAVE_M_DIR} _OCTAVE_M_DIR)
  #message(STATUS "(CMake) _OCTAVE_M_DIR = ${_OCTAVE_M_DIR}")
  
  #OCTAVE_OCT_DIR
  if(NOT DEFINED OCTAVE_OCT_DIR)
    file(WRITE ${CMAKE_BINARY_DIR}/octave_command
      "printf(octave_config_info(\"localoctfiledir\"));"
      )
    execute_process(
      COMMAND ${OCTAVE} -q -f octave_command
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      OUTPUT_VARIABLE OCTAVE_OCT_DIR
      )
    #message(STATUS "OCTAVE_OCT_DIR = ${OCTAVE_OCT_DIR}")
    file(TO_CMAKE_PATH ${OCTAVE_OCT_DIR} OCTAVE_OCT_DIR)
    #message(STATUS "(CMake) OCTAVE_OCT_DIR = ${OCTAVE_OCT_DIR}")
  endif(NOT DEFINED OCTAVE_OCT_DIR)
  
  # Replace the OCTAVE_PREFIX with the PLplot prefix in OCTAVE_M_DIR
  string(REPLACE
  "${OCTAVE_PREFIX}" 
  "${CMAKE_INSTALL_PREFIX}"
  OCTAVE_M_DIR
  ${_OCTAVE_M_DIR}
  )
  message(STATUS "OCTAVE_M_DIR = ${OCTAVE_M_DIR}")
  
  # Transform OCTAVE_OCT_DIR if prefixes not the same.
  if(NOT CMAKE_INSTALL_PREFIX STREQUAL "${OCTAVE_PREFIX}")
    set(OCTAVE_OCT_DIR ${CMAKE_INSTALL_LIBDIR}/octave)
  endif(NOT CMAKE_INSTALL_PREFIX STREQUAL "${OCTAVE_PREFIX}") 
  message(STATUS "OCTAVE_OCT_DIR = ${OCTAVE_OCT_DIR}")
  
endif(ENABLE_octave)
