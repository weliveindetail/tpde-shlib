// Test loading the TPDE shared library and verify X86TargetMachine registration

// RUN: env TPDE_DEBUG=On %clang -c %s -o %t.o \
// RUN:        -mllvm -print-after-all 2>&1 | FileCheck --check-prefix=CHECK-REGULAR %s
// CHECK-REGULAR-NOT: Running TPDE pass
// CHECK-REGULAR: Machine code for function

// RUN: env TPDE_DEBUG=On %clang -c %s -o %t.o -Xclang -load -Xclang %tpde_shlib \
// RUN:        -mllvm -print-after-all 2>&1 | FileCheck  --check-prefix=CHECK-TPDE %s
// CHECK-TPDE: Running TPDE pass
// CHECK-TPDE-NOT: Machine code for function

int test() { return 0; }
