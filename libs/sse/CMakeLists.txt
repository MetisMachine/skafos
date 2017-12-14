cmake_minimum_required(VERSION 3.0)

if (CMAKE_VERSION VERSION_LESS "3")
	project(sse C)
else()
	cmake_policy(SET CMP0048 NEW)
	project(sse VERSION 0.2 LANGUAGES C)
endif()

if(NOT ${CMAKE_VERSION} LESS 3.2)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  message(STATUS "Checking compiler flags for C++11 support.")
  include(CheckCXXCompilerFlag)

  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
  check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

  if(COMPILER_SUPPORTS_CXX11)
      message(STATUS "C++11 is supported.")

      if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
      else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      endif()
  elseif(COMPILER_SUPPORTS_CXX0X)
      message(STATUS "C++0x is supported.")

      if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
          set(CMAKE_MACOSX_RPATH 1)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
      else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
      endif()
  else()
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
endif()


FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/libs/sse/parse-sse.c)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${FlexOutput}
      COMMAND ${FLEX_EXECUTABLE}
	          -I --outfile=${FlexOutput}
              ${CMAKE_SOURCE_DIR}/libs/sse/parse-sse.fl
      COMMENT "Generating parse-sse.c"
    )
ENDIF()

add_library(sse http.c ${FlexOutput} post.c sse.c tools.c)
target_include_directories(sse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(sse PRIVATE -Wall -fPIC)

