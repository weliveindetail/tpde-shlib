#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/raw_ostream.h"

class TPDEPass : public llvm::ModulePass {
public:
  static char ID;

  TPDEPass(llvm::CodeGenTargetMachineImpl *FallbackTM,
           llvm::raw_pwrite_stream &Out, llvm::raw_pwrite_stream *DwoOut,
           llvm::CodeGenFileType FileType, bool DisableVerify,
           llvm::MachineModuleInfoWrapperPass *MMIWP)
      : llvm::ModulePass(ID), FallbackTM(FallbackTM), OS(Out), DwoOS(DwoOut),
        FileType(FileType), DisableVerify(DisableVerify), MMIWP(MMIWP) {}
  bool runOnModule(llvm::Module &M) override;
  bool fallbackToLLVM(llvm::Module &M);

  llvm::StringRef getPassName() const override {
    return "Single-pass TPDE backend";
  }

private:
  llvm::CodeGenTargetMachineImpl *FallbackTM;
  llvm::raw_pwrite_stream &OS;
  llvm::raw_pwrite_stream *DwoOS;
  llvm::CodeGenFileType FileType;
  bool DisableVerify;
  llvm::MachineModuleInfoWrapperPass *MMIWP;
};
