include "llvm/Target/Target.td"

include "YASarchRegisterInfo.td"
include "YASarchInstrInfo.td"

def YASarchInstrInfo : InstrInfo;

def YASarch : Target {
    let InstructionSet = YASarchInstrInfo;
}