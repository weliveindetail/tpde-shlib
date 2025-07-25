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

add_subdirectory(deps/tpde)

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
  VERSION ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}
  SOVERSION ${LLVM_VERSION_MAJOR}
)

add_subdirectory(test)
