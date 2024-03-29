# examples/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2012 Alan W. Irwin
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

# N.B. This file is used for both the core build (which installs the examples
# and optionally [depending on BUILD_TEST} builds them) and the installed
# examples build.  The core build has BUILD_TEST OFF or ON at user option
# and CORE_BUILD always ON.  The installed examples build always has
# BUILD_TEST ON and CORE_BUILD OFF.

set(f95_STRING_INDICES
  "00"
  "01"
  "02"
  "03"
  "04"
  "05"
  "06"
  "07"
  "08"
  "09"
  "10"
  "11"
  "12"
  "13"
  "14"
  "15"
  "16"
  "16a"
  "17"
  "18"
  "19"
  "20"
  "21"
  "22"
  "23"
  "24"
  "25"
  "26"
  "27"
  "28"
  "29"
  "30"
  "31"
  "33"
  )

if(CORE_BUILD)
  # For CMake-2.6.0 and above, the module files are created by
  # default during the library build in the bindings/f95 directory.
  include_directories(${CMAKE_BINARY_DIR}/bindings/f95)
else(CORE_BUILD)
  include_directories(${INCLUDE_DIR} ${F95_MOD_DIR})
endif(CORE_BUILD)

if(CORE_BUILD)
  set(plf95demolib${LIB_TAG}_LIB_SRCS plf95demolib.f90)

  add_library(plf95demolib${LIB_TAG} STATIC ${plf95demolib${LIB_TAG}_LIB_SRCS})
  target_link_libraries(plf95demolib${LIB_TAG} plplotf95${LIB_TAG})

  install(TARGETS plf95demolib${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )
  install(FILES
    ${CMAKE_BINARY_DIR}/examples/f95/plf95demolib.mod
    DESTINATION ${F95_MOD_DIR}
    )

  set(f95_SRCS)
  foreach(STRING_INDEX ${f95_STRING_INDICES})
    set(f95_SRCS ${f95_SRCS} x${STRING_INDEX}f.f90)
  endforeach(STRING_INDEX ${f95_STRING_INDICES})

  # Process fortran header file to define isnan
  if(FORTRAN_HAVE_ISNAN)
    set(HAVE_F95_ISNAN_FALSE !)
  else(FORTRAN_HAVE_ISNAN)
    set(HAVE_F95_ISNAN_TRUE !)
  endif(FORTRAN_HAVE_ISNAN)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plf95demos.inc.in
    ${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
    )
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
    PROPERTIES GENERATED ON)
  include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

  install(FILES ${f95_SRCS}
    DESTINATION ${DATA_DIR}/examples/f95
    )

  set(FC ${CMAKE_Fortran_COMPILER})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    DESTINATION ${DATA_DIR}/examples/f95
    RENAME Makefile
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
    DESTINATION ${DATA_DIR}/examples/f95
    )

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/f95
    )
endif(CORE_BUILD)

if(BUILD_TEST)

  set_property(GLOBAL PROPERTY TARGETS_examples_f95)
  foreach(STRING_INDEX ${f95_STRING_INDICES})
    # Need unique target name
    add_executable(x${STRING_INDEX}f95 x${STRING_INDEX}f.f90)
    set_target_properties(
      x${STRING_INDEX}f95 PROPERTIES
      OUTPUT_NAME x${STRING_INDEX}f
      )
    target_link_libraries(x${STRING_INDEX}f95 plf95demolib${LIB_TAG} plplotf95${LIB_TAG} plplotf95c${LIB_TAG})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_f95 x${STRING_INDEX}f95)
  endforeach(STRING_INDEX ${f95_STRING_INDICES})
endif(BUILD_TEST)
