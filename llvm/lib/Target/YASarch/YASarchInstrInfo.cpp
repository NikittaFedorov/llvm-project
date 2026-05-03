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
