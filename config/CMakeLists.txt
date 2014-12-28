##=========================================================================
## This file is part of GanitaGraph software. 
## 
##=========================================================================

project(GanitaZero CXX C)

## Version
set(GanitaZero_VERSION_MAJOR "0")
set(GanitaZero_VERSION_MINOR "1")
set(GanitaZero_VERSION_PATCH "0")

cmake_minimum_required(VERSION 2.8)

  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wconversion -O3 ${OpenMP_CXX_FLAGS} -msse4")

  enable_testing()

  IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_64)
    set(CPACK_TOPLEVEL_TAG x86_64)
  ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_32)
    set(CPACK_TOPLEVEL_TAG x86_32)
  ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")

# configure_file(config.h.in config.h)

include_directories(../src/cpp)
link_directories (/usr/local/lib)

add_library(GanitaZero ../src/cpp/GanitaZeroSchema.cpp ../src/cpp/GanitaZeroRepresentations.cpp ../src/cpp/GanitaZero.cpp ../src/cpp/GanitaZeroVersion.cpp)

target_include_directories (GanitaZero PUBLIC ../src/cpp)

add_executable(zerog ../src/drivers/zerog.cc)
add_test(zerog ../src/drivers/zerog.cc)
# add_executable(ganitaGraph ../src/drivers/ganitaGraph.cc)
# add_test(ganitaGraph ../src/drivers/ganitaGraph.cc)

target_link_libraries (zerog LINK_PUBLIC GanitaGraph GanitaZero)
# target_link_libraries (gg2 LINK_PUBLIC GanitaZero)

# install(TARGETS ganitaGraph DESTINATION bin)
# install(TARGETS GanitaZero DESTINATION lib)

# if(OpenMP_CXX_FLAGS)
#   set_property(
#     SOURCE hmm_test.cpp
#     PROPERTY COMPILE_DEFINITIONS WITH_OMP
#     )
# ENDIF(OpenMP_CXX_FLAGS)
