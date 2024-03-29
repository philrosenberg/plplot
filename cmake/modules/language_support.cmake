# cmake/modules/language_support.cmake
#
# Temporary additional general language support is contained within this
# file.  

# This additional function definition is needed to provide a workaround for
# CMake bug 9220.

option(ENABLE_workaround_9220 "Enable a workaround for cmake bug 9220" ON)

if(ENABLE_workaround_9220)
  function(workaround_9220 language language_works)
    #message("DEBUG: language = ${language}")
    set(text
      "project(test C)
# Locations where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/modules
  ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake
  ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake-2.8
  )

# Need to define CMAKE_PLATFORM_INFO_DIR for all CMake versions less than 2.8.10
if(NOT CMAKE_PLATFORM_INFO_DIR)
  set(CMAKE_PLATFORM_INFO_DIR \${CMAKE_BINARY_DIR}\${CMAKE_FILES_DIRECTORY})
  message(STATUS 'CMAKE_PLATFORM_INFO_DIR = \${CMAKE_PLATFORM_INFO_DIR}')
endif(NOT CMAKE_PLATFORM_INFO_DIR)

if(CMAKE_SYSTEM_NAME STREQUAL \"Linux\")
  # We prefer to support only the latest CMake version because it
  # tends to be more free of issues and more consistent with recent
  # software releases of software that PLplot depends on than earlier versions.
  # However, as a special concession to our modern (not enterprise or
  # LTS) Linux distro users so they can avoid a download of recent CMake,
  # we allow them to use the system version of
  # CMake which at this time (2011-10) is typically 2.8.2 or later.
  cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)
else(CMAKE_SYSTEM_NAME STREQUAL \"Linux\")
  # For non-Linux platforms we prefer to support only the most recent
  # CMake version since that tends to be most free of issues and most
  # consistent with releases of software that PLplot depends on.  The
  # most recently released CMake is 2.8.6 at this time (2011-10).
  cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)
endif(CMAKE_SYSTEM_NAME STREQUAL \"Linux\")
message(STATUS \"CMAKE_GENERATOR = ${CMAKE_GENERATOR}\")
#enable_language(${language} OPTIONAL)
enable_language(${language})
"
      )
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/language_tests/${language})
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language})
    file(WRITE ${CMAKE_BINARY_DIR}/language_tests/${language}/CMakeLists.txt
      ${text})
    # Special language support files for various languages:
    # N.B. This list of files has to be maintained to be consistent
    # with anything special we do in terms of language support.
    
    if(language STREQUAL "Ada")
      set(language_special ON)
      set(language_files
	language_support/cmake/CMakeAdaCompiler.cmake.in
	language_support/cmake/CMakeAdaInformation.cmake
	language_support/cmake/CMakeDetermineAdaCompiler.cmake
	language_support/cmake/CMakeTestAdaCompiler.cmake
	)
    elseif(language STREQUAL "D")
      set(language_special ON)
      set(language_files
	language_support/cmake/CMakeDCompiler.cmake.in
	language_support/cmake/CMakeDInformation.cmake
	language_support/cmake/CMakeDetermineDCompiler.cmake
	language_support/cmake/CMakeTestDCompiler.cmake
	language_support/cmake/Platform/Linux-dmd.cmake
	language_support/cmake/Platform/Linux-gdc.cmake
	language_support/cmake/Platform/Windows-dmd.cmake
	language_support/cmake/Platform/Windows-gdc.cmake
	)
    elseif(language STREQUAL "Fortran")
      set(language_special OFF)
    else(language STREQUAL "Ada")
      set(language_special OFF)
    endif(language STREQUAL "Ada")

    if(language_special)
      file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language}/cmake/modules/Platform)
      foreach(file ${language_files})
	configure_file(
	  ${CMAKE_SOURCE_DIR}/cmake/modules/${file}
	  ${CMAKE_BINARY_DIR}/language_tests/${language}/cmake/modules/${file}
	  COPYONLY
	  )
      endforeach(file ${language_files})
    endif(language_special)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language}
      RESULT_VARIABLE return_code
      #OUTPUT_VARIABLE output
      #ERROR_VARIABLE error
      #OUTPUT_STRIP_TRAILING_WHITESPACE
      #ERROR_STRIP_TRAILING_WHITESPACE
      )
    if(return_code EQUAL 0)
      set(${language_works} ON CACHE INTERNAL "")
    else(return_code EQUAL 0)
      set(${language_works} OFF CACHE INTERNAL "")
      #message(STATUS "A test cmake run with language = ${language} enabled failed with the following stdout and stderr:")
      #message(STATUS "stdout = ${output}")
      #message(STATUS "stderr = ${error}")
    endif(return_code EQUAL 0)
  endfunction(workaround_9220)
else(ENABLE_workaround_9220)
  function(workaround_9220 language language_works)
    # Dummy version that always returns success.
    set(${language_works} ON CACHE INTERNAL "")
  endfunction(workaround_9220)
endif(ENABLE_workaround_9220)

# Temporary tests of the above function.
#workaround_9220(CXX CXX_language_works)
#message("CXX_language_works = ${CXX_language_works}")
#workaround_9220(CXXp CXXp_language_works)
#message("CXXp_language_works = ${CXXp_language_works}")
#workaround_9220(Ada Ada_language_works)
#message("Ada_language_works = ${Ada_language_works}")
#workaround_9220(D D_language_works)
#message("D_language_works = ${D_language_works}")
#workaround_9220(Fortran Fortran_language_works)
#message("Fortran_language_works = ${Fortran_language_works}")
#workaround_9220(Java Java_language_works)
#message("Java_language_works = ${Java_language_works}")
# Just to terminate temporary test for convenience.
#message(FATAL_ERROR "")
