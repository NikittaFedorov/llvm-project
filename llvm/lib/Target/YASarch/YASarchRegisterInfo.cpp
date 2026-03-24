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

const MCPhysReg *
YASarchRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  YASarch_DUMP_GREEN
  return CSR_YASarch_SaveList;
}

BitVector YASarchRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  YASarch_DUMP_GREEN
  YASarchFrameLowering const *TFI = getFrameLowering(MF);

  BitVector Reserved(getNumRegs());
  Reserved.set(YASarch::R1);

  if (TFI->hasFP(MF)) {
    Reserved.set(YASarch::R2);
  }
  return Reserved;
}

bool YASarchRegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return false;
}

bool YASarchRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  YASarch_DUMP_GREEN
  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

Register YASarchRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  YASarch_DUMP_GREEN
  const TargetFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? YASarch::R2 : YASarch::R1;
}

const uint32_t *
YASarchRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                      CallingConv::ID CC) const {
  YASarch_DUMP_GREEN
  return CSR_YASarch_RegMask;
}