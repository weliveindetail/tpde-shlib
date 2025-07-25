// Test that we can link TPDE-compiled objects into valid executables

// RUN: %clang %s -O3 -o %t.exe -Xclang -load -Xclang %tpde_shlib
// RUN: %t.exe | FileCheck %s
// CHECK: Running TPDE-compiled executable

#include <cstdio>

int main() {
  printf("Running TPDE-compiled executable");
  return 0;
}
