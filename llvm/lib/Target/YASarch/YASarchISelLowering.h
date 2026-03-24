#pragma once

#include "YASarch.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class YASarchSubtarget;
class YASarchTargetMachine;

namespace YASarchISD {

enum NodeType : unsigned {
  // Start the numbering where the builtin ops and target ops leave off.
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET,
  CALL,
  BR_CC,
};

} // namespace YASarchISD

class YASarchTargetLowering : public TargetLowering {
public:
  explicit YASarchTargetLowering(const TargetMachine &TM, const YASarchSubtarget &STI);

  /// This method returns the name of a target specific DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

  YASarchSubtarget const &getSubtarget() const { return STI; }

private:
  const YASarchSubtarget &STI;
};


} // end namespace llvm