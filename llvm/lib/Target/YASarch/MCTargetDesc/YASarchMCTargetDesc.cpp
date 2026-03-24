#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/YASarchTargetInfo.h"
#include "YASarch.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "YASarchGenRegisterInfo.inc"

static MCRegisterInfo *createYASarchMCRegisterInfo(const Triple &TT) {
  YASarch_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitYASarchMCRegisterInfo(X, YASarch::R0);
  return X;
}

// We need to define this function
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYASarchTargetMC() {
    YASarch_DUMP_MAGENTA
    Target &TheYASarchTarget = getTheYASarchTarget();
    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(TheYASarchTarget, createYASarchMCRegisterInfo);
}