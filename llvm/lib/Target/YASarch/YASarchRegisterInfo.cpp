#include "llvm/CodeGen/TargetInstrInfo.h"

#include "YASarch.h"
#include "YASarchRegisterInfo.h"
#include "YASarchFrameLowering.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "YASarchGenRegisterInfo.inc"

YASarchRegisterInfo::YASarchRegisterInfo() : YASarchGenRegisterInfo(YASarch::R0) {
  YASarch_DUMP_GREEN
}