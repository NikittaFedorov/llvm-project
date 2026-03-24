#pragma once

#include "llvm/CodeGen/TargetFrameLowering.h"

#include "YASarch.h"

namespace llvm {

class YASarchSubtarget;

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
                    MachineBasicBlock &MBB) const override {}
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {}

  /// hasFP - Return true if the specified function should have a dedicated
  /// frame pointer register. For most targets this is true only if the function
  /// has variable sized allocas or if frame pointer elimination is disabled.
  bool hasFPImpl(const MachineFunction &MF) const override { return false; }

private:
  const YASarchSubtarget &STI;
};

} // namespace llvm
