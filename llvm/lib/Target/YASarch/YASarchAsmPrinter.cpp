#include "llvm/ADT/Statistic.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"

#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include "YASarch.h"
#include "YASarchSubtarget.h"
#include "YASarchTargetMachine.h"
#include "TargetInfo/YASarchTargetInfo.h"
#include "MCTargetDesc/YASarchInstrPrinter.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {

class YASarchAsmPrinter : public AsmPrinter {
  const MCSubtargetInfo *STI;

public:
  explicit YASarchAsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), STI(TM.getMCSubtargetInfo()) {
    YASarch_DUMP_GREEN
  }

  void emitInstruction(const MachineInstr *MI) override;

  StringRef getPassName() const override { return "YASarch Assembly Printer"; }

  bool lowerPseudoInstExpansion(const MachineInstr *MI, MCInst &Inst);
};

} // end anonymous namespace

// Simple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "YASarchGenMCPseudoLowering.inc"

void YASarchAsmPrinter::emitInstruction(const MachineInstr *MI) {
  YASarch_DUMP_GREEN
  // Do any auto-generated pseudo lowerings.
  if (MCInst OutInst; lowerPseudoInstExpansion(MI, OutInst)) {
    EmitToStreamer(*OutStreamer, OutInst);
    return;
  }
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYASarchAsmPrinter() {
  RegisterAsmPrinter<YASarchAsmPrinter> X(getTheYASarchTarget());
}