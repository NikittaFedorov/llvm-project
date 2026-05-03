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

static const MachineInstrBuilder &addFrameReference(const MachineInstrBuilder &MIB,
                                                    int FI) {
  MachineInstr *MI = MIB;
  MachineFunction &MF = *MI->getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const MCInstrDesc &MCID = MI->getDesc();
  MachineMemOperand::Flags Flags = MachineMemOperand::MONone;
  if (MCID.mayLoad())
    Flags |= MachineMemOperand::MOLoad;
  if (MCID.mayStore())
    Flags |= MachineMemOperand::MOStore;

  MachineMemOperand *MMO = MF.getMachineMemOperand(
      MachinePointerInfo::getFixedStack(MF, FI), Flags, MFI.getObjectSize(FI),
      MFI.getObjectAlign(FI));
  return MIB.addFrameIndex(FI).addImm(0).addMemOperand(MMO);
}

YASarchInstrInfo::YASarchInstrInfo() : YASarchGenInstrInfo() { YASarch_DUMP_GREEN }

void YASarchInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MBBI,
                                   const DebugLoc &DL, MCRegister DstReg,
                                   MCRegister SrcReg, bool KillSrc,
                                   bool RenamableDest,
                                   bool RenamableSrc) const {
  if (YASarch::GPRRegClass.contains(DstReg, SrcReg)) {
    BuildMI(MBB, MBBI, DL, get(YASarch::ORI), DstReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
    return;
  }
  llvm_unreachable("can't copyPhysReg");
}

void YASarchInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI, Register SrcReg,
    bool IsKill, int FrameIdx, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg,
    MachineInstr::MIFlag Flags) const {
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  assert(RC == &YASarch::GPRRegClass && "Unsupported regclass to store");
  addFrameReference(BuildMI(MBB, MBBI, DL, get(YASarch::ST))
                        .addReg(SrcReg, getKillRegState(IsKill)),
                    FrameIdx);
}

void YASarchInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI, Register DestReg,
    int FrameIdx, const TargetRegisterClass *RC, const TargetRegisterInfo *TRI,
    Register VReg, MachineInstr::MIFlag Flags) const {
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  assert(RC == &YASarch::GPRRegClass && "Unsupported regclass to load");
  addFrameReference(BuildMI(MBB, MBBI, DL, get(YASarch::LD), DestReg),
                    FrameIdx);
}
