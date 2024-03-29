# cmake/modules/csiro.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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

# Module for determining all configuration variables for the CSIRO libraries.

option(PL_HAVE_QHULL "Enable use of the Qhull library" ON)
option(WITH_CSA "Enable use of the csa library" ON)

# This logic copied verbatim from csiro.ac for ix86 systems and alpha systems
# with two possible compilers.  In future, this logic will need to be
# expanded to a lot more cases as we gain platform experience.
set(NAN_CFLAGS ${CMAKE_C_FLAGS})
if(PL_HAVE_QHULL OR WITH_CSA)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "i[0-9]86")
    set(NAN_CFLAGS "${NAN_CFLAGS} -mieee-fp")
  else(CMAKE_SYSTEM_PROCESSOR MATCHES "i[0-9]86")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "alpha.*")
      if(CMAKE_C_COMPILER MATCHES "gcc")
        set(NAN_CFLAGS "${NAN_CFLAGS} -mieee")
      else(CMAKE_C_COMPILER MATCHES "gcc")
        set(NAN_CFLAGS "${NAN_CFLAGS} -ieee")
      endif(CMAKE_C_COMPILER MATCHES "gcc")
    endif(CMAKE_SYSTEM_PROCESSOR MATCHES "alpha.*")
  endif(CMAKE_SYSTEM_PROCESSOR MATCHES "i[0-9]86")
  if(NOT DEFINED NaNAwareCCompiler)
    message(STATUS "Check for NaN awareness in C compiler")
    try_run(NaNAwareCCompiler COMPILE_RESULT
      ${CMAKE_BINARY_DIR}
      ${CMAKE_SOURCE_DIR}/cmake/modules/TestNaNAware.c
      CMAKE_FLAGS "-DCOMPILE_DEFINITIONS:STRING=${NAN_CFLAGS} -I\"${CMAKE_SOURCE_DIR}\"/lib/csa ${MATH_LIB}"
      OUTPUT_VARIABLE OUTPUT
      )
    if(NOT COMPILE_RESULT)
      set(NaNAwareCCompiler ${COMPILE_RESULT})
    endif(NOT COMPILE_RESULT)
    if(NaNAwareCCompiler)
      message(STATUS "Check for NaN awareness in C compiler - found")
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
	"Determining whether C compiler is NaN aware succeeded with "
	"the following output:\n${OUTPUT}\n\n"
	)
    else(NaNAwareCCompiler)
      message(STATUS "Check for NaN awareness in C compiler - not found")
      message(STATUS "WARNING: Setting PL_HAVE_QHULL and WITH_CSA to OFF.")
      set(PL_HAVE_QHULL OFF CACHE BOOL "Enable use of the Qhull library" FORCE)
      set(WITH_CSA OFF CACHE BOOL "Enable use of the csa library" FORCE)
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
	"Determining whether C compiler is NaN aware failed with "
	"the following output:\n${OUTPUT}\n\n"
	)
    endif(NaNAwareCCompiler)
  endif(NOT DEFINED NaNAwareCCompiler)
endif(PL_HAVE_QHULL OR WITH_CSA)

if(PL_HAVE_QHULL)
  find_package(QHULL)
  if(QHULL_FOUND)
    include(CheckFunctionExists)
    set(CMAKE_REQUIRED_LIBRARIES ${QHULL_LIBRARIES})
    check_function_exists(qh_new_qhull QH_NEW_EXIST)
    set(CMAKE_REQUIRED_LIBRARIES)
    if(QH_NEW_EXIST)
      message(STATUS "QHULL_INCLUDE_DIRS = ${QHULL_INCLUDE_DIRS}")
      message(STATUS "QHULL_LIBRARIES = ${QHULL_LIBRARIES}")
      set(QHULL_RPATH ${QHULL_LIBRARY_DIRS})
      filter_rpath(QHULL_RPATH)
      message(STATUS "QHULL_RPATH = ${QHULL_RPATH}")
    else(QH_NEW_EXIST)
      message(STATUS "WARNING: function qh_new_qhull not found.  Setting PL_HAVE_QHULL to OFF.")
      set(PL_HAVE_QHULL OFF CACHE BOOL "Enable use of the Qhull library" FORCE)
    endif(QH_NEW_EXIST)
  else(QHULL_FOUND)
    message(STATUS
      "WARNING: qhull library not found.  Setting PL_HAVE_QHULL to OFF."
      )
    set(PL_HAVE_QHULL OFF CACHE BOOL "Enable use of the Qhull library" FORCE)
  endif(QHULL_FOUND)
endif(PL_HAVE_QHULL)
