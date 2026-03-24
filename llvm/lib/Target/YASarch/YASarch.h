#pragma once

#include "llvm/Support/raw_ostream.h"

#include "llvm/Target/TargetMachine.h"

#include "MCTargetDesc/YASarchMCTargetDesc.h"

#define YASarch_DUMP(Color)                                                        \
  {                                                                            \
    llvm::errs().changeColor(Color)                                            \
        << __func__ << "\n\t\t" << __FILE__ << ":" << __LINE__ << "\n";        \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  }
// #define YASarch_DUMP(Color) {}

#define YASarch_DUMP_RED     YASarch_DUMP(llvm::raw_ostream::RED)
#define YASarch_DUMP_GREEN   YASarch_DUMP(llvm::raw_ostream::GREEN)
#define YASarch_DUMP_YELLOW  YASarch_DUMP(llvm::raw_ostream::YELLOW)
#define YASarch_DUMP_CYAN    YASarch_DUMP(llvm::raw_ostream::CYAN)
#define YASarch_DUMP_MAGENTA YASarch_DUMP(llvm::raw_ostream::MAGENTA)
#define YASarch_DUMP_WHITE   YASarch_DUMP(llvm::raw_ostream::WHITE)

namespace llvm {
class YASarchTargetMachine;
class FunctionPass;
class YASarchSubtarget;
class AsmPrinter;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;

bool lowerYASarchMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                  AsmPrinter &AP);
bool LowerYASarchMachineOperandToMCOperand(const MachineOperand &MO,
                                       MCOperand &MCOp, const AsmPrinter &AP);

FunctionPass *createYASarchISelDag(YASarchTargetMachine &TM, CodeGenOptLevel OptLevel);

} // namespace llvm