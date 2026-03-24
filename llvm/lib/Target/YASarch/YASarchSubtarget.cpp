
#include "YASarchSubtarget.h"
#include "YASarch.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "sim-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "YASarchGenSubtargetInfo.inc"

YASarchSubtarget::YASarchSubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                           const StringRef &FS, const TargetMachine &TM)
    : YASarchGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {
  YASarch_DUMP_CYAN
}