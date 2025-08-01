#include "TPDEPass.h"
#include "Utils.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

#include "tpde-llvm/LLVMCompiler.hpp"

#include <memory>

using namespace llvm;

char TPDEPass::ID;

constexpr bool PassFail = true;
constexpr bool PassSuccess = false;

bool TPDEPass::fallbackToLLVM(Module &M) {
  if (!FallbackTM) {
    errs() << "LLVM fallback not configured: aborting\n";
    return PassFail;
  }

  legacy::PassManager PM;
  MCContext *Ctx;
  if (FallbackTM->addPassesToEmitMC(PM, Ctx, OS)) {
    errs() << "Target does not support MC emission: " << M.getTargetTriple()
           << "\n";
    return PassFail;
  }

  errs() << "Falling back to LLVM: " << M.getName() << "\n";
  PM.run(M);

  return PassSuccess;
}

bool TPDEPass::runOnModule(Module &M) {
  std::string ModName = M.getName().str();
  errs() << "Running TPDE pass on module " << ModName << "\n";

  auto Compiler = tpde_llvm::LLVMCompiler::create(Triple(M.getTargetTriple()));
  if (!Compiler)
    return PassFail; // Triple unsupported

  std::vector<uint8_t> Elf;
  if (!Compiler->compile_to_elf(M, Elf))
    return fallbackToLLVM(M);

  // Compilation successful: write out buffer
  OS.pwrite(reinterpret_cast<char *>(Elf.data()), Elf.size(), 0);
  return PassSuccess;
}
