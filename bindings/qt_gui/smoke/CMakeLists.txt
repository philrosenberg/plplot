# bindings/qt_gui/smoke/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2014 Hazen Babcock
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

if(ENABLE_smoke)

  set(plplot_smoke_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/smokedata.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/x_1.cpp
    )
  set_source_files_properties(
    ${plplot_smoke_SRC}
    PROPERTIES
    GENERATED ON
    COMPILE_FLAGS "-DUSINGDLL"
    )

  add_custom_command(
    OUTPUT ${plplot_smoke_SRC}
    COMMAND ${SMOKE_GEN_BIN} 
    ARGS -config "${SMOKE_GEN_SHARED}/qt-config.xml" -smokeconfig "${CMAKE_CURRENT_SOURCE_DIR}/smokeconfig.xml" -I "${CMAKE_SOURCE_DIR}/include" -- ${CMAKE_CURRENT_SOURCE_DIR}/plplotqt.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/smokeconfig.xml ${CMAKE_CURRENT_SOURCE_DIR}/plplotqt.h
    )
  add_custom_target(generate_smoke_source
    DEPENDS ${plplot_smoke_SRC}
    )

  message("DEBUG: SMOKE_GEN_BIN = ${SMOKE_GEN_BIN}")
  message("DEBUG: SMOKE_GEN_SHARED = ${SMOKE_GEN_SHARED}")

  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_SOURCE_DIR}/lib/nistcd
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )

  add_library(smokeplplotqt${LIB_TAG} ${plplot_smoke_SRC})

  target_link_libraries(
    smokeplplotqt${LIB_TAG}
    plplotqt${LIB_TAG}
    plplot${LIB_TAG}
    ${QT_LIBRARIES}
    )

  add_dependencies(smokeplplotqt${LIB_TAG} generate_smoke_source)

  # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
  set_qt_target_properties(smokeplplotqt${LIB_TAG})

  if(USE_RPATH)

    set_target_properties(
      smokeplplotqt${LIB_TAG}
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )

  else(USE_RPATH)
    set_target_properties(
      smokeplplotqt${LIB_TAG}
      PROPERTIES
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(TARGETS smokeplplotqt${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

endif(ENABLE_smoke)
