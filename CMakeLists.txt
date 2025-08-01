cmake_minimum_required(VERSION 3.20.0)

project(tpde-shlib)

#list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED LLVM_MAIN_SRC_DIR)
  cmake_minimum_required(VERSION 3.20.0)
  project(llvm-autojit)
  set(TPDE_SHLIB_STANDALONE_BUILD True)

  find_package(LLVM REQUIRED CONFIG)
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

  include_directories(${LLVM_INCLUDE_DIRS})
  separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
  add_definitions(${LLVM_DEFINITIONS_LIST})

  set(LLVM_NO_RTTI 1)
endif()

if(NOT TARGET tpde)
  add_subdirectory(deps/tpde)
endif()

add_llvm_library(tpde-shlib SHARED
  tpde-shlib.cpp
  TPDEPass.cpp

  LINK_COMPONENTS
    Core
    Passes
    Support

  LINK_LIBS
    tpde_llvm
)

set_target_properties(tpde-shlib PROPERTIES
  PREFIX ""
  OUTPUT_NAME tpde
  VERSION ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}
  SOVERSION ${LLVM_VERSION_MAJOR}
)

add_subdirectory(test)

install(PROGRAMS bench-tpde.sh
  DESTINATION .
  COMPONENT tpde-bench-scripts
)

add_custom_target(install-tpde-bench
  COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --component tpde-bench-scripts
  COMMENT "Installing TPDE benchmark"
)

set(bench_deps tpde-shlib clang clang-resource-headers lld)
if(LLVM_LINK_LLVM_DYLIB)
  list(APPEND bench_deps clang-cpp LLVM)
endif()

foreach(dep IN ITEMS ${bench_deps})
  add_dependencies(install-tpde-bench "install-${dep}")
endforeach()
