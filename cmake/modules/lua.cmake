# cmake/modules/lua.cmake
#
# Lua binding configuration
#
# Copyright (C) 2008  Werner Smekal
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

# Module for determining Lua bindings configuration options

# Options to enable Lua bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_lua "Enable Lua bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_lua "Enable Lua bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_lua AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Lua requires shared libraries. Disabling Lua bindings")
  set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
endif(ENABLE_lua AND NOT BUILD_SHARED_LIBS)

if(ENABLE_lua AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling Lua bindings")
  set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
endif(ENABLE_lua AND NOT SWIG_FOUND)

if(ENABLE_lua)
  transform_version(NUMERICAL_SWIG_MINIMUM_VERSION_FOR_LUA "1.3.35")
  transform_version(NUMERICAL_SWIG_VERSION "${SWIG_VERSION}")
  if(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_SWIG_MINIMUM_VERSION_FOR_LUA}")
    message(STATUS "WARNING: swig version too old for Lua bindings.   Disabling Lua bindings.")
    set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
  endif(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_SWIG_MINIMUM_VERSION_FOR_LUA}")
endif(ENABLE_lua)

if(ENABLE_lua AND NOT PL_DOUBLE)
  message(STATUS "WARNING: "
    "only single precision floating point. Disabling Lua bindings")
  set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
endif(ENABLE_lua AND NOT PL_DOUBLE)

if(ENABLE_lua)
  # Check for Lua libraries which defines
  #  LUA_LIBRARIES    = path to the Lua library
  #  LUA_INCLUDE_DIR  = path to where lua.h is found
  #  and LUA_FOUND consistently.
  find_package(Lua)
  if(NOT LUA_FOUND)
    message(STATUS "WARNING: "
      "Lua library and/or header not found. Disabling Lua bindings")
    set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
  endif(NOT LUA_FOUND)
endif(ENABLE_lua)

if(ENABLE_lua)
  find_program(LUA_EXECUTABLE lua)
  if(NOT LUA_EXECUTABLE)
    message(STATUS "WARNING: "
      "Lua executable not found. Disabling Lua bindings")
    set(ENABLE_lua OFF CACHE BOOL "Enable Lua bindings" FORCE)
  endif(NOT LUA_EXECUTABLE)
endif(ENABLE_lua)

if(ENABLE_lua)
  # Unless some better convention comes along, follow what Debian does for
  # install location of Lua wrapper shared object.
  if(LUA_VERSION MATCHES "5\\.0")
    set(LUA_DIR ${LIB_DIR}/lua/50/plplot)
  else(LUA_VERSION MATCHES "5\\.0")
    set(LUA_DIR ${LIB_DIR}/lua/${LUA_VERSION}/plplot)
  endif(LUA_VERSION MATCHES "5\\.0")
endif(ENABLE_lua)
