#include "TPDEPass.h"

#include "llvm/IR/Module.h"
#include "llvm/TargetParser/Triple.h"

#include "tpde-llvm/LLVMCompiler.hpp"

#include <memory>

using namespace llvm;

char TPDEPass::ID;

bool TPDEPass::runOnModule(Module &M) {
  std::string ModName = M.getName().str();
  printf("Running TPDE pass on module %s\n", ModName.c_str());

  auto Compiler = tpde_llvm::LLVMCompiler::create(Triple(M.getTargetTriple()));
  std::vector<uint8_t> Elf;
  if (Compiler && Compiler->compile_to_elf(M, Elf)) {
    // Compilation successful: write out buffer
    OS.pwrite(reinterpret_cast<char *>(Elf.data()), Elf.size(), 0);
    return true;
  }

  // Triple unsupported or compilation failed
  return false;
}
