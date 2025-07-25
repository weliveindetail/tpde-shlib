// Test loading the TPDE shared library and verify X86TargetMachine registration

// RUN: %clang -c %s -o %t.o -Xclang -load -Xclang %tpde_shlib 2>&1 | FileCheck %s
// CHECK: Running TPDE pass

int test() { return 0; }
