cmake_minimum_required(VERSION 3.0)

project(skafos VERSION 0.0.1)

set(CPACK_PACKAGE_VERSION 0.0.1)

set(EXECUTABLE_OUTPUT_PATH "bin")
set(BUILD_SHARED_LIBS NO)

option(COLLECT_FILES    "Copy libraries to <build>/lib, binaries to <build>/bin, headers to <build>/include." ON)
option(COMPILE_EXAMPLES "Compile examples." OFF)

set(RUNTIME_OUTPUT_DIRECTORY       "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_DIRECTORY       "${PROJECT_BINARY_DIR}/lib")
set(ARCHIVE_OUTPUT_DIRECTORY       "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)

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

add_subdirectory(src)
add_subdirectory(libs)

FIND_PACKAGE(LibArchive REQUIRED)
FIND_PACKAGE(CURL REQUIRED)
FIND_PACKAGE(Libgit2 REQUIRED)
FIND_PACKAGE(YamlCpp REQUIRED)

file(GLOB_RECURSE HEADERS 
    "libs/restclient-cpp/include/**/*.h"
    "src/**/*.h" 
    "src/*.h"
)

file(GLOB_RECURSE SOURCES 
    "libs/restclient-cpp/source/*.cc"
    "src/**/*.cpp" 
    "src/*.cpp"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wwritable-strings -Wunneeded-internal-declaration")

include_directories(  
    SYSTEM
    ${CURL_INCLUDE_DIRS}
    ${LibArchive_INCLUDE_DIRS}
    ${YAMLCPP_INCLUDE_DIR}
    ${LIBGIT2_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/yaml-cpp
    ${CMAKE_SOURCE_DIR}/libs/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/libs/json11
    ${CMAKE_SOURCE_DIR}/libs/cpr/include
    ${CMAKE_SOURCE_DIR}/libs/sse
    ${CMAKE_SOURCE_DIR}/libs/termcolor
    ${CMAKE_SOURCE_DIR}/libs/restclient-cpp/include
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(skafos ${SOURCES})
target_link_libraries(skafos docopt json11 sse yaml-cpp ${CURL_LIBRARIES} ${LIBGIT2_LIBRARIES} ${LibArchive_LIBRARIES} ${YAMLCPP_LIBRARY})

install(TARGETS skafos RUNTIME DESTINATION bin)
