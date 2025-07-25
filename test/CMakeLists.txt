# Configure lit site configuration
llvm_canonicalize_cmake_booleans(
  LLVM_ENABLE_PLUGINS
)

set(LLVM_LIT_ARGS "-vv --no-progress-bar")

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
)

set(TPDE_SHLIB_TEST_DEPENDS
  tpde-shlib
  llvm-config
  clang
  FileCheck
  count
  not
)

add_lit_testsuite(check-tpde-shlib "Running the TPDE custom backend tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${TPDE_SHLIB_TEST_DEPENDS}
)
