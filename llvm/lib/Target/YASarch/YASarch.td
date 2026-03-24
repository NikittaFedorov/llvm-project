include "llvm/Target/Target.td"

include "YASarchRegisterInfo.td"
include "YASarchInstrInfo.td"

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

def YASarchInstrInfo : InstrInfo;

def YASarch : Target {
    let InstructionSet = YASarchInstrInfo;
}