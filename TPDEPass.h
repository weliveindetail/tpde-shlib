#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

class TPDEPass : public llvm::ModulePass {
public:
  static char ID;

  TPDEPass(llvm::raw_pwrite_stream &Out) : llvm::ModulePass(ID), OS(Out) {}
  bool runOnModule(llvm::Module &M) override;

  llvm::StringRef getPassName() const override {
    return "Single-pass TPDE backend";
  }

private:
  llvm::raw_pwrite_stream &OS;
};
