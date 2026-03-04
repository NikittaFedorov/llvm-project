#include "TargetInfo/YASarchTargetInfo.h"
#include "YASarch.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheYASarchTarget() {
  YASarch_DUMP_YELLOW
  static Target TheYASarchTarget;
  return TheYASarchTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYASarchTargetInfo() {
  YASarch_DUMP_YELLOW
  RegisterTarget<Triple::YASarch> X(getTheYASarchTarget(), "YASarch",
                                "YASarch Simulator target for LLVM course", "YASarch");
}