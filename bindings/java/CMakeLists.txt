# bindings/java/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

if(ENABLE_java)

  # Swig generated java files.  Order no longer matters since class 
  # dependencies are explicitly handled now.

  set(
    SWIG_JAVA_FILES
    plplotjavacJNI.java 
    PLGraphicsIn.java 
    plplotjavacConstants.java
    plplotjavac.java
    )

  # List of swig-generated + configured java files
  set(
    JAVA_GEN_FILES
    config.java
    ${SWIG_JAVA_FILES}
    )

  # List of swig-generated + configured java files with full path names
  # Need this otherwise cmake will look in the source directory for the
  # .java files.
  string(
    REGEX REPLACE "([a-zA-z]*)\\.java"
    "${CMAKE_CURRENT_BINARY_DIR}/\\1.java" 
    JAVA_GEN_FILES_FULL 
    "${JAVA_GEN_FILES}"
    )

  # Full pathnames for all java files.
  set(
    JAVA_FILES_FULL
    ${JAVA_GEN_FILES_FULL}
    ${CMAKE_CURRENT_SOURCE_DIR}/PLStream.java
    ${CMAKE_CURRENT_SOURCE_DIR}/PLCallbackMapform.java
    ${CMAKE_CURRENT_SOURCE_DIR}/PLCallbackLabel.java
    ${CMAKE_CURRENT_SOURCE_DIR}/PLCallbackCT.java

    )

  # Explicit full-path class dependencies for the foreach loop below which
  # should build classes in the correct order regardless of whether it
  # is a parallel build or not and regardless of the order of JAVA_FILES_FULL.
  # These are hand-crafted dependencies based on scanning the appropriate
  # Java sources.  Apparently CMake has a java dependency scanner which
  # might be good enough to do this task as well, but I have avoided it because
  # I (AWI) am under the impression that CMake support of java is still in its
  # infancy.

  set(class_root ${CMAKE_CURRENT_BINARY_DIR}/plplot/core)

  set(
    ${class_root}/plplotjavacJNI.class_DEPENDS
    ${class_root}/PLCallbackMapform.class
    ${class_root}/PLCallbackLabel.class
    ${class_root}/PLCallbackCT.class
    )

  set(
    ${class_root}/PLGraphicsIn.class_DEPENDS
    ${class_root}/plplotjavacJNI.class
    )

  set(
    ${class_root}/plplotjavacConstants.class_DEPENDS
    ${class_root}/plplotjavacJNI.class
    )

  set(
    ${class_root}/plplotjavac.class_DEPENDS
    ${class_root}/plplotjavacConstants.class
    ${class_root}/PLGraphicsIn.class
    ${class_root}/plplotjavacJNI.class
    ${class_root}/PLCallbackMapform.class
    ${class_root}/PLCallbackLabel.class
    ${class_root}/PLCallbackCT.class
    )

  set(
    ${class_root}/PLStream.class_DEPENDS
    ${class_root}/plplotjavacConstants.class
    ${class_root}/config.class
    ${class_root}/plplotjavac.class
    ${class_root}/PLCallbackMapform.class
    ${class_root}/PLCallbackLabel.class
    ${class_root}/PLCallbackCT.class
    )

  # This is currently the include list for swig, the C wrapper and the
  # the java classpath. Not particular pretty...
  set(java_interface_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${JAVA_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/bindings/swig-support
    )
  # On some systems JAVA_INCLUDE_PATH2 returns JAVA_INCLUDE_PATH2-NOTFOUND
  if(JAVA_INCLUDE_PATH2)
    set(java_interface_INCLUDE_PATHS
      ${java_interface_INCLUDE_PATHS}
      ${JAVA_INCLUDE_PATH2}
      )
  endif(JAVA_INCLUDE_PATH2)
  include_directories(${java_interface_INCLUDE_PATHS})

  # Can't use source file properties as we have to quote the flags in that 
  # case and it breaks swig. Doh! I would call this a cmake bug.
  if(SWIG_JAVA_NOPGCPP)
    #This version of swig supports the -nopgcpp option
    set(CMAKE_SWIG_FLAGS -DPL_DOUBLE_INTERFACE -DSWIG_JAVA -package plplot.core -nopgcpp)
  else(SWIG_JAVA_NOPGCPP)
    set(CMAKE_SWIG_FLAGS -DPL_DOUBLE_INTERFACE -DSWIG_JAVA -package plplot.core)
  endif(SWIG_JAVA_NOPGCPP)

  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

  # This may be needed in future when we use CMake java support to build the
  # class files, but it obviously is not correct now when we use custom commands
  # to build the class files using file dependencies to keep things straight.
  ## set_source_files_properties(${JAVA_GEN_FILES_FULL} PROPERTIES GENERATED ON)

  set(SWIG_MODULE_plplotjavac_wrap_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i)

  set_source_files_properties(
    plplotjavac.i 
    PROPERTIES SWIG_MODULE_NAME plplotjavac
    )

  # Create swig-generated java files and swig-generated C wrapper source
  # code.  Also, build that source code into a shared object (a.k.a. plug-in)
  # that can be dynamically loaded by java.
  swig_add_module(plplotjavac_wrap java plplotjavac.i)
  # The shared object must be linked against the core PLplot library.
  swig_link_libraries(plplotjavac_wrap plplot${LIB_TAG})

  # Create config.java.  Other generated java files created by swig above.
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.java.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.java
    )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    set_target_properties(
      plplotjavac_wrap
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${JAVAWRAPPER_HARDDIR}"
      )
  else(USE_RPATH)
    set_target_properties(
      plplotjavac_wrap
      PROPERTIES
      INSTALL_NAME_DIR "${JAVAWRAPPER_HARDDIR}"
      )
  endif(USE_RPATH)

  # Ensure we get the correct suffix for OS-X 
  if(APPLE)
    set_target_properties(
      plplotjavac_wrap
      PROPERTIES
      SUFFIX ".dylib"
      )
  endif(APPLE)

  install(TARGETS plplotjavac_wrap LIBRARY DESTINATION ${JAVAWRAPPER_HARDDIR})

  set(JAVA_CLASSES)
  foreach( srcfile ${JAVA_FILES_FULL} )
    get_filename_component(fileroot ${srcfile} NAME_WE)
    set(output_file ${CMAKE_CURRENT_BINARY_DIR}/plplot/core/${fileroot}.class)
    list(APPEND JAVA_CLASSES ${output_file})
    add_custom_command(
      OUTPUT ${output_file}
      COMMAND ${CMAKE_Java_COMPILER} 
      ${JAVACFLAGS} -classpath ${CMAKE_CURRENT_BINARY_DIR} ${srcfile} -d ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS ${srcfile} ${${output_file}_DEPENDS}
      VERBATIM
      )
  endforeach( srcfile ${JAVA_FILES_FULL} )
  add_custom_target(plplot_core ALL DEPENDS ${JAVA_CLASSES})

  # Need to keep track of file dependencies since this is a custom target.
  set_property(GLOBAL PROPERTY FILES_plplot_core ${JAVA_CLASSES})

  # Ensure that swig is executed before we try to compile the java
  # classes which file-depend (see above custom_command DEPENDS) on
  # the java files created by swig (amongst others).
  add_dependencies(plplot_core plplotjavac_wrap)

  # Installed as part of the example/java directory
  #install(FILES ${CMAKE_BINARY_DIR}/plplot.jar DESTINATION ${JAR_DIR})

endif(ENABLE_java)
