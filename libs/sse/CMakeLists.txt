cmake_minimum_required(VERSION 2.8)
if (CMAKE_VERSION VERSION_LESS "3")
	project(sse C)
else()
	cmake_policy(SET CMP0048 NEW)
	project(sse VERSION 0.2 LANGUAGES C)
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

