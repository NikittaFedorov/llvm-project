#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

#include "YASarchInstrInfo.h"
#include "YASarch.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "YASarchGenInstrInfo.inc"

#define DEBUG_TYPE "YASarch-inst-info"

YASarchInstrInfo::YASarchInstrInfo() : YASarchGenInstrInfo() { YASarch_DUMP_GREEN }