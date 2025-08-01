#include "TPDEPass.h"
#include "Utils.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/X86/X86TargetMachine.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

constexpr bool AddPassesFail = true;
constexpr bool AddPassesSuccess = false;

class X86TargetMachineTPDE : public X86TargetMachine {
public:
  X86TargetMachineTPDE(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       std::optional<Reloc::Model> RM,
                       std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                       bool JIT)
      : X86TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT) {}

  bool
  addPassesToEmitFile(PassManagerBase &PM, raw_pwrite_stream &Out,
                      raw_pwrite_stream *DwoOut, CodeGenFileType FileType,
                      bool DisableVerify = true,
                      MachineModuleInfoWrapperPass *MMIWP = nullptr) override {
    CodeGenTargetMachineImpl *BaseTM = static_cast<X86TargetMachine *>(this);
    PM.add(new TPDEPass(BaseTM, Out, DwoOut, FileType, DisableVerify, MMIWP));
    return false; // success
  }
};

class X86TargetMachineCustomLLVM : public X86TargetMachine {
public:
  X86TargetMachineCustomLLVM(const Target &T, const Triple &TT, StringRef CPU,
                             StringRef FS, const TargetOptions &Options,
                             std::optional<Reloc::Model> RM,
                             std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                             bool JIT)
      : X86TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT) {}

  bool
  addPassesToEmitFile(PassManagerBase &PM, raw_pwrite_stream &Out,
                      raw_pwrite_stream *DwoOut, CodeGenFileType FileType,
                      bool DisableVerify = true,
                      MachineModuleInfoWrapperPass *MMIWP = nullptr) override {
    errs() << "Running custom LLVM backend\n";

    if (!MMIWP)
      MMIWP = new MachineModuleInfoWrapperPass(this);

    TargetPassConfig *PassConfig = createPassConfig(PM);
    PassConfig->setDisableVerify(DisableVerify);
    PM.add(PassConfig);
    PM.add(MMIWP);

    if (PassConfig->addISelPasses())
      return AddPassesFail;
    PassConfig->addMachinePasses();
    PassConfig->setInitialized();

    if (TargetPassConfig::willCompleteCodeGenPipeline()) {
      if (addAsmPrinter(PM, Out, DwoOut, FileType, MMIWP->getMMI().getContext()))
        return AddPassesFail;
    } else {
      // MIR printing is redundant with -filetype=null.
      if (FileType != CodeGenFileType::Null)
        PM.add(createPrintMIRPass(Out));
    }

    PM.add(createFreeMachineFunctionPass());
    return AddPassesSuccess;
  }
};

namespace llvm {

LLVM_ABI Target &getTheX86_32Target();
LLVM_ABI Target &getTheX86_64Target();

} // namespace llvm

struct PatchLLVMX86TargetOnLoad {
  PatchLLVMX86TargetOnLoad() {
    Target &x86_32 = getTheX86_32Target();
    Target &x86_64 = getTheX86_64Target();

    // cl::opt causes static init race and won't work reliably :(
    if (getEnvBool("USE_CUSTOM_LLVM")) {
      RegisterTargetMachine<X86TargetMachineCustomLLVM> X(x86_32);
      RegisterTargetMachine<X86TargetMachineCustomLLVM> Y(x86_64);
    } else {
      RegisterTargetMachine<X86TargetMachineTPDE> X(x86_32);
      RegisterTargetMachine<X86TargetMachineTPDE> Y(x86_64);
    }
  }
};

PatchLLVMX86TargetOnLoad SetupTPDEFor;
