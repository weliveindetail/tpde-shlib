#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include <optional>

using namespace llvm;

class X86TargetMachineTPDE : public TargetMachine {
public:
  X86TargetMachineTPDE(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       std::optional<Reloc::Model> RM,
                       std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                       bool JIT)
      : TargetMachine(T, "", TT, CPU, FS, Options) {}

  bool
  addPassesToEmitFile(PassManagerBase &PM, raw_pwrite_stream &Out,
                      raw_pwrite_stream *DwoOut, CodeGenFileType FileType,
                      bool DisableVerify = true,
                      MachineModuleInfoWrapperPass *MMIWP = nullptr) override {
    // success
    return false;
  }
};

namespace llvm {

Target &getTheX86_32Target();
Target &getTheX86_64Target();

} // namespace llvm

struct PatchLLVMX86TargetOnLoad {
  PatchLLVMX86TargetOnLoad() {
    llvm::RegisterTargetMachine<X86TargetMachineTPDE>(getTheX86_32Target());
    llvm::RegisterTargetMachine<X86TargetMachineTPDE>(getTheX86_64Target());
  }
};

PatchLLVMX86TargetOnLoad SetupTPDEFor;
