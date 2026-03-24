#pragma once

#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include <optional>


#include "YASarchSubtarget.h"

namespace llvm {
extern Target TheYASarchTarget;

class YASarchTargetMachine : public CodeGenTargetMachineImpl {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  YASarchSubtarget Subtarget;

public:
  YASarchTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                   bool JIT);

  const YASarchSubtarget *getSubtargetImpl(const Function &) const override {
    YASarch_DUMP_CYAN
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;
};
} // end namespace llvm
