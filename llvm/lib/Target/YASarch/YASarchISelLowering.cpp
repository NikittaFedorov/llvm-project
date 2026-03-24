#include "YASarchISelLowering.h"
#include "YASarch.h"
#include "YASarchRegisterInfo.h"
#include "YASarchSubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFunction.h"

#define DEBUG_TYPE "YASarch-lower"

using namespace llvm;

YASarchTargetLowering::YASarchTargetLowering(const TargetMachine &TM,
                                     const YASarchSubtarget &STI)
    : TargetLowering(TM), STI(STI) {
  YASarch_DUMP_RED
  addRegisterClass(MVT::i32, &YASarch::GPRRegClass);
}

const char *YASarchTargetLowering::getTargetNodeName(unsigned Opcode) const {
  YASarch_DUMP_RED
  switch (Opcode) {
  case YASarchISD::CALL:
    return "YASarchISD::CALL";
  case YASarchISD::RET:
    return "YASarchISD::RET";
  }
  return nullptr;
}