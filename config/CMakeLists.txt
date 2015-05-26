##=========================================================================
## This is a configuration file for building GanitaZero. 
## 
##=========================================================================

project(GanitaZero CXX C)

## Version
set(GanitaZero_VERSION_MAJOR "0")
set(GanitaZero_VERSION_MINOR "16")
set(GanitaZero_VERSION_PATCH "2")

cmake_minimum_required(VERSION 2.8)

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CC")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lGanitaZero -lGanitaGraph" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lGanitaZero -lGanitaGraph" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "clang++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lGanitaZero -lGanitaGraph" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lGanitaZero -lGanitaGraph" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "c++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(warnings "/W4 /WX /EHsc")
else()
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++11")
endif()

if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${warnings} ${cmake_cxx_extra}"
    CACHE STRING "Flags used by the compiler during build." FORCE)
  set(CMAKE_C_FLAGS "${warnings} ${cmake_cxx_extra}"
    CACHE STRING "Flags used by the compiler during build." FORCE)
endif()

  IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_64)
    set(CPACK_TOPLEVEL_TAG x86_64)
  ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_32)
    set(CPACK_TOPLEVEL_TAG x86_32)
  ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")

# configure_file(config.h.in config.h)

include_directories(/usr/local/include ../src/include)
link_directories (/usr/local/lib)

add_library(GanitaZero SHARED ../src/cpp/GanitaZeroSchema.cpp ../src/cpp/GanitaZeroRepresentations.cpp ../src/cpp/GanitaZero.cpp ../src/cpp/GanitaZeroVersion.cpp ../src/cpp/GanitaZeroHist.cpp ../src/cpp/GanitaZeroTile.cpp)

target_include_directories (GanitaZero PUBLIC /usr/local/include ../src/include ../src/cpp)

add_executable(gzero ../src/drivers/gzero.cc)
add_executable(gp ../src/drivers/gPredict.cc)
add_test(gztest gzero ../sample/Chacon3.gztran)

target_link_libraries (gzero GanitaZero GanitaGraph m ${CMAKE_SHARED_LINKER_FLAGS})
target_link_libraries (gp GanitaZero GanitaGraph m ${CMAKE_SHARED_LINKER_FLAGS})

install(TARGETS GanitaZero DESTINATION lib)
install(TARGETS gzero DESTINATION bin)
install(TARGETS gp DESTINATION bin)
FILE(GLOB files "../src/include/ganita/zero/*.hpp")
install(FILES ${files} DESTINATION include/ganita/zero)
install(DIRECTORY ../src/include/ganita/zero/representations DESTINATION include/ganita/zero)

# if(OpenMP_CXX_FLAGS)
#   set_property(
#     SOURCE hmm_test.cpp
#     PROPERTY COMPILE_DEFINITIONS WITH_OMP
#     )
# ENDIF(OpenMP_CXX_FLAGS)
