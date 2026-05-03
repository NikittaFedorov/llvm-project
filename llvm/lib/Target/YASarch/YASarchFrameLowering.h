#pragma once

#include "YASarchInstrInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/MathExtras.h"

#include "YASarch.h"

namespace llvm {

class YASarchSubtarget;

static inline void buildStackAdjust(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MBBI,
                                    const YASarchInstrInfo &TII,
                                    const DebugLoc &DL, int64_t Amount,
                                    MachineInstr::MIFlag Flag) {
  if (Amount == 0)
    return;

  if (isInt<16>(Amount)) {
    BuildMI(MBB, MBBI, DL, TII.get(YASarch::ADDI), YASarch::R1)
        .addReg(YASarch::R1)
        .addImm(Amount)
        .setMIFlag(Flag);
    return;
  }

  int64_t Hi = static_cast<int16_t>((Amount >> 16) & 0xffff);
  int64_t Lo = static_cast<int16_t>(Amount & 0xffff);

  BuildMI(MBB, MBBI, DL, TII.get(YASarch::MOVHI), YASarch::R15)
      .addImm(Hi)
      .setMIFlag(Flag);
  BuildMI(MBB, MBBI, DL, TII.get(YASarch::ORI), YASarch::R15)
      .addReg(YASarch::R15)
      .addImm(Lo)
      .setMIFlag(Flag);
  BuildMI(MBB, MBBI, DL, TII.get(YASarch::ADD), YASarch::R1)
      .addReg(YASarch::R1)
      .addReg(YASarch::R15)
      .setMIFlag(Flag);
}

class YASarchFrameLowering : public TargetFrameLowering {
public:
    YASarchFrameLowering(const YASarchSubtarget &STI)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0),
        STI(STI) {
    YASarch_DUMP_GREEN
  }

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {
    uint64_t FrameSize = MF.getFrameInfo().getStackSize();
    if (!FrameSize)
      return;

    auto MBBI = MBB.begin();
    DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    const auto &TII =
        static_cast<const YASarchInstrInfo &>(*MF.getSubtarget().getInstrInfo());
    buildStackAdjust(MBB, MBBI, TII, DL, -static_cast<int64_t>(FrameSize),
                     MachineInstr::FrameSetup);
  }
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {
    uint64_t FrameSize = MF.getFrameInfo().getStackSize();
    if (!FrameSize)
      return;

    auto MBBI = MBB.getLastNonDebugInstr();
    DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    const auto &TII =
        static_cast<const YASarchInstrInfo &>(*MF.getSubtarget().getInstrInfo());
    buildStackAdjust(MBB, MBBI, TII, DL, static_cast<int64_t>(FrameSize),
                     MachineInstr::FrameDestroy);
  }

  /// hasFP - Return true if the specified function should have a dedicated
  /// frame pointer register. For most targets this is true only if the function
  /// has variable sized allocas or if frame pointer elimination is disabled.
  bool hasFPImpl(const MachineFunction &MF) const override { return false; }

private:
  const YASarchSubtarget &STI;
};

} // namespace llvm
