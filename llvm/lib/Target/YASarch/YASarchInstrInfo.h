#pragma once

#include "YASarchRegisterInfo.h"
#include "MCTargetDesc/YASarchInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "YASarchGenInstrInfo.inc"

namespace llvm {

class YASarchSubtarget;

class YASarchInstrInfo : public YASarchGenInstrInfo {
public:
  YASarchInstrInfo();

};

} // end namespace llvm