; Check that modules with unsupported features fall back to LLVM
;
; RUN: %clang -c %s -o %t.o -Xclang -load -Xclang %tpde_shlib 2>&1 | FileCheck %s
; CHECK: Falling back to LLVM

define void @g() {
entry:
  %0 = load x86_fp80, ptr null, align 16
  ret void
}
