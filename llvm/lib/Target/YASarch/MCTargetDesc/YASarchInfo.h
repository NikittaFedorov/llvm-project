#pragma once

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace YASarchOp {
enum OperandType : unsigned {
  OPERAND_SIMM16 = MCOI::OPERAND_FIRST_TARGET,
};
} // namespace YASarchOp

} // end namespace llvm