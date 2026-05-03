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
  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc, bool RenamableDest = false,
                   bool RenamableSrc = false) const override;
};

} // end namespace llvm
