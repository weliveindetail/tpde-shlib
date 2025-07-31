// Test that we can link TPDE-compiled objects into valid executables

// RUN: env USE_CUSTOM_LLVM=On \
// RUN:     %clang %s -O3 -o %t.exe -Xclang -load -Xclang %tpde_shlib \
// RUN:               -mllvm -print-after-all 2>&1 | FileCheck %s
// CHECK: Running custom LLVM backend
//
// RUN: %t.exe | FileCheck --check-prefix=CHECK-EXEC %s
// CHECK-EXEC: Running custom-LLVM-compiled executable

#include <cstdio>

int main() {
  printf("Running custom-LLVM-compiled executable");
  return 0;
}
