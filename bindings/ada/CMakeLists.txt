# bindings/ada/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007 Alan W. Irwin
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

if(ENABLE_ada)

  set(SOURCE_LIST
    plplot.adb
    plplot.ads
    plplot_thin.adb
    plplot_thin.ads
    plplot_traditional.adb
    plplot_traditional.ads
    plplot_auxiliary.adb
    plplot_auxiliary.ads
    )
  
  # These are Ada library information files built by gnat.  I am not 
  # sure whether the name suffixes correspond to the *.adb or *.ads files
  # above or the union of them.  In any case, if any of the names change
  # above, then this list will probably have to be changed as well.)
  # N.B. the absolute location prefix of these files may have to be changed
  # in the future since this is a CMake internal.
  set(ALI_PREFIX 
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/plplotada${LIB_TAG}.dir
    )
  set(ALI_LIST
    ${ALI_PREFIX}/plplot.ali
    ${ALI_PREFIX}/plplot_thin.ali
    ${ALI_PREFIX}/plplot_traditional.ali
    ${ALI_PREFIX}/plplot_auxiliary.ali
    )

  # I am pretty sure this is a workaround for a MinGW gnatmake bug
  # since the installation of these files is not needed for Linux.
  # However, without this workaround the standard examples build on
  # MinGW/MSYS platforms without problems (so gnatmake is happy if the
  # *.o files are not installed which is another indication we are
  # dealing with a bug here), but at run-time the ada examples
  # immediately return with a return code of 3.
  if(MINGW)
    list(APPEND ALI_LIST
      ${ALI_PREFIX}/plplot.o
      ${ALI_PREFIX}/plplot_thin.o
      ${ALI_PREFIX}/plplot_traditional.o
      ${ALI_PREFIX}/plplot_auxiliary.o
      )
  endif(MINGW)

  set(plplotada${LIB_TAG}_LIB_SRCS)
  foreach(SOURCE_FILE ${SOURCE_LIST})
    list(APPEND plplotada${LIB_TAG}_LIB_SRCS 
      ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}
      )
  endforeach(SOURCE_FILE ${SOURCE_LIST})

  add_library(plplotada${LIB_TAG} ${plplotada${LIB_TAG}_LIB_SRCS})

  if(NON_TRANSITIVE)
    # empty list ==> non-transitive linking for everything that links to
    # libplplotada${LIB_TAG} in the shared libraries case.
    target_link_libraries(plplotada${LIB_TAG} LINK_INTERFACE_LIBRARIES)
    # This configures the pkg-config method to use non-transitive linking.
    set(PC_REQUIRES_TAG "Requires.private")
  else(NON_TRANSITIVE)
    # This configures the pkg-config method to use transitive linking
    set(PC_REQUIRES_TAG "Requires")
  endif(NON_TRANSITIVE)

  # Work around an issue in our CMake Ada language
  # support for MinGW/Windows.  FIXME.  This issue should be fixed
  # at the Ada language support level and not worked around here.
  if(USE_DLL_SUBDIRECTORY)
    add_custom_command(
      TARGET plplotada${LIB_TAG} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      libplplotada${LIB_TAG}.dll libplplotada${LIB_TAG}.dll.a
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dll
      VERBATIM
      )
  endif(USE_DLL_SUBDIRECTORY)

  target_link_libraries(plplotada${LIB_TAG}
    plplot${LIB_TAG} ${GNAT_LIB}
    )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    set_target_properties(plplotada${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotada_SOVERSION}
      VERSION ${plplotada_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotada${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotada_SOVERSION}
      VERSION ${plplotada_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(FILES ${plplotada${LIB_TAG}_LIB_SRCS} 
    DESTINATION ${ADA_INCLUDE_DIR}
    )
  
  # Permissions of *.ali files in accordance with
  # http://people.debian.org/~lbrenta/debian-ada-policy.html
  install(FILES ${ALI_LIST}
    DESTINATION ${ADA_LIB_DIR}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )
  
  install(TARGETS plplotada${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # Configure pkg-config *.pc file corresponding to libplplotada${LIB_TAG}
  if(PKG_CONFIG_EXECUTABLE)
    if(LIB_TAG)
      set(PC_PRECISION "double")
    else(LIB_TAG)
      set(PC_PRECISION "single")
    endif(LIB_TAG)
    # Each list element must consist of a colon-separated string with the
    # following fields which are parsed out in the foreach loop below and
    # used to configure the corresponding pkg-config *.pc file.
    # BINDING 	      	  - ENABLE_${BINDING} keeps track of whether a
    # 			    binding has been enabled (ON) or not (OFF).
    #			    Also, ${BINDING} used to determine PC_FILE_SUFFIX
    #			    which helps to determine name of configured 
    #			    *.pc file.
    # PC_SHORT_NAME	  - Used in *.pc NAME: field
    # PC_LONG_NAME	  - Used in *.pc Description: field
    # PC_LIBRARY_NAME	  - Used in *.pc Libs: field
    #                         Also used to determine PC_LINK_FLAGS and
    #                         PC_COMPILE_FLAGS used in *.pc Libs: and Cflags:
    #			    fields.
    set(PC_DATA "ada:Ada:Ada bindings, :plplotada${LIB_TAG}")
    
    string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
    set(PC_FILE_SUFFIX "-${BINDING}")
    set(PC_REQUIRES "plplot${LIB_TAG}")
    string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
    set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
    set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
    if(NON_TRANSITIVE)
      set(PC_LINK_FLAGS "-lplplot${LIB_TAG} ${PC_LINK_FLAGS}")
    endif(NON_TRANSITIVE)
    set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
    set(PC_CONFIGURED_FILE
      ${CMAKE_BINARY_DIR}/pkgcfg/plplot${LIB_TAG}${PC_FILE_SUFFIX}.pc
      )
    configure_file(
      ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.in
      ${PC_CONFIGURED_FILE}
      @ONLY
      )
    install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
  endif(PKG_CONFIG_EXECUTABLE)

  # Add generated .ali files to the list of additional files to be
  # removed by make clean
  SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ALI_LIST}")

endif(ENABLE_ada)
