#include "llvm/MC/TargetRegistry.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/MCAsmInfo.h"

#include <optional>

#include "YASarchTargetMachine.h"
#include "MCTargetDesc/YASarchMCTargetDesc.h"
#include "TargetInfo/YASarchTargetInfo.h"

#include "YASarch.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYASarchTarget() {
  // Register the target.
  YASarch_DUMP_CYAN
  RegisterTargetMachine<YASarchTargetMachine> A(getTheYASarchTarget());
}

YASarchTargetMachine::YASarchTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32",
                               TT, CPU, FS, Options, Reloc::Static,
                               getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  YASarch_DUMP_CYAN
  initAsmInfo();
  if (!MRI)
    MRI.reset(createYASarchMCRegisterInfo(TT));
  if (!MII)
    MII.reset(createYASarchMCInstrInfo());
  if (!STI)
    STI.reset(createYASarchMCSubtargetInfo(TT, CPU, FS));
  if (!AsmInfo) {
    MCAsmInfo *TmpAsmInfo = createYASarchMCAsmInfo(*MRI, TT, Options.MCOptions);

    if (Options.BinutilsVersion.first > 0)
      TmpAsmInfo->setBinutilsVersion(Options.BinutilsVersion);

    if (Options.DisableIntegratedAS) {
      TmpAsmInfo->setUseIntegratedAssembler(false);
      TmpAsmInfo->setParseInlineAsmUsingAsmParser(false);
    }

    TmpAsmInfo->setPreserveAsmComments(Options.MCOptions.PreserveAsmComments);
    TmpAsmInfo->setFullRegisterNames(Options.MCOptions.PPCUseFullRegisterNames);

    if (Options.ExceptionModel != ExceptionHandling::None)
      TmpAsmInfo->setExceptionsType(Options.ExceptionModel);

    AsmInfo.reset(TmpAsmInfo);
  }
}

namespace {

/// YASarch Code Generator Pass Configuration Options.
class YASarchPassConfig : public TargetPassConfig {
public:
  YASarchPassConfig(YASarchTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  YASarchTargetMachine &getYASarchTargetMachine() const {
    return getTM<YASarchTargetMachine>();
  }

  bool addInstSelector() override {
    YASarch_DUMP_CYAN
    addPass(createYASarchISelDag(getYASarchTargetMachine(), getOptLevel()));
    return false;
  }
};

} // end anonymous namespace

TargetPassConfig *YASarchTargetMachine::createPassConfig(PassManagerBase &PM) {
  YASarch_DUMP_CYAN
  return new YASarchPassConfig(*this, PM);
}

TargetLoweringObjectFile *YASarchTargetMachine::getObjFileLowering() const {
  YASarch_DUMP_CYAN
  return TLOF.get();
}
