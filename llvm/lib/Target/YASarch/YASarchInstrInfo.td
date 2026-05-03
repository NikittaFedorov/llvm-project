//===----------------------------------------------------------------------===//
// YASarch profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_YASarchBrCC : SDTypeProfile<0, 2, [SDTCisVT<0, i32>,
                                           SDTCisVT<1, OtherVT>]>;

def YASarch_brcc : SDNode<"YASarchISD::BR_CC", SDT_YASarchBrCC,
                          [SDNPHasChain]>;

def YASarch_ret      : SDNode<"YASarchISD::RET", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "YASarchInstrFormats.td"

//===----------------------------------------------------------------------===//
// YASarch Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

class SImmAsmOperand<int width> : AsmOperandClass {
  let Name = "SImm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{return isInt<16>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getSImm16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM16";
  let OperandNamespace = "YASarchOp";
}

def btarget16 : Operand<OtherVT> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let PrintMethod = "printBranchOperand";
  let EncoderMethod = "getBranchTarget16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_PCREL";
}

def LI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffff,
                                   SDLoc(N), MVT::i32);
}]>;

def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, SDLoc(N),
                                   MVT::i32);
}]>;

//===----------------------------------------------------------------------===//
// Instructions specific format
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class IncCCi<bits<8> op>
    : YASarchInst<op, (outs GPR:$r1, GPR:$r2_), (ins GPR:$r2, simm16:$r3_imm),
                  NAME#" $r1 $r2 $r3_imm", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
multiclass Arithm<bits<8> opcode, SDPatternOperator op> {
  def NAME : YASarchInst<opcode, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         NAME#" $r1 $r2 $r3_imm",
                         [(set GPR:$r1, (op GPR:$r2, GPR:$r3_imm))]>;
  def NAME#I
      : YASarchInst<!add(opcode, 0x10), (outs GPR:$r1),
                    (ins GPR:$r2, simm16:$r3_imm),
                    NAME#"i $r1 $r2 $r3_imm",
                    [(set GPR:$r1, (op GPR:$r2, simm16:$r3_imm))]>;
}

//===----------------------------------------------------------------------===//
// YASarch Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1, r1 = 0, r2 = 0 in
def B : YASarchInst<0xB0, (outs), (ins btarget16:$r3_imm), "B $r3_imm",
                    [(br bb:$r3_imm)]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, r2 = 0, r3_imm = 0 in
def BR : YASarchInst<0xBB, (outs), (ins GPR:$r1), "BR $r1",
                 [(brind GPR:$r1)]>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def PseudoRET : YASarchPseudoInst<(outs), (ins), "# PseudoRET",
                [(YASarch_ret)]>,
                PseudoInstExpansion<(BR R0)>;

def INC_EQi : IncCCi<0x90>;
def INC_NEi : IncCCi<0x91>;
def INC_LEi : IncCCi<0x92>;
def INC_LTi : IncCCi<0x93>;
def INC_GEi : IncCCi<0x94>;
def INC_GTi : IncCCi<0x95>;

let isBranch = 1, isTerminator = 1, r2 = 0 in
def BR_COND : YASarchInst<0xBC, (outs), (ins GPR:$r1, btarget16:$r3_imm),
                          "BR_COND $r1 $r3_imm",
                          [(YASarch_brcc GPR:$r1, bb:$r3_imm)]>;

// movli
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0 in
def MOVLI : YASarchInst<0xAA, (outs GPR:$r1), (ins simm16:$r3_imm),
                    "MOVli $r1 $r3_imm",
                    [(set GPR:$r1, simm16:$r3_imm)]>;

let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0 in
def MOVHI : YASarchInst<0xAF, (outs GPR:$r1), (ins simm16:$r3_imm),
                        "MOVhi $r1 $r3_imm", []>;

defm ADD : Arithm<0x30, add>;
defm SUB : Arithm<0x31, sub>;
defm MUL : Arithm<0x32, mul>;
defm DIV : Arithm<0x33, sdiv>;
defm OR : Arithm<0x34, or>;
defm AND : Arithm<0x35, and>;
defm XOR : Arithm<0x36, xor>;
defm SRA : Arithm<0x37, sra>;
defm SRL : Arithm<0x38, srl>;
defm SHL : Arithm<0x39, shl>;
defm REM : Arithm<0x3A, srem>;

let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def LD : YASarchInst<0x70, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                     "LD $r1 $r2 $r3_imm", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
def ST : YASarchInst<0x71, (outs), (ins GPR:$r1, GPR:$r2, simm16:$r3_imm),
                     "ST $r1 $r2 $r3_imm", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_EQi : YASarchInst<0x50, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_EQi $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_NEi : YASarchInst<0x51, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_NEi $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_LEi : YASarchInst<0x52, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_LEi $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_LTi : YASarchInst<0x53, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_LTi $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_GEi : YASarchInst<0x54, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_GEi $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_GTi : YASarchInst<0x55, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                          "CMP_GTi $r1 $r2 $r3_imm", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_EQ : YASarchInst<0x60, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_EQ $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_NE : YASarchInst<0x61, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_NE $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_LE : YASarchInst<0x62, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_LE $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_LT : YASarchInst<0x63, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_LT $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_GE : YASarchInst<0x64, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_GE $r1 $r2 $r3_imm", []>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def CMP_GT : YASarchInst<0x65, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                         "CMP_GT $r1 $r2 $r3_imm", []>;

// flush
let r1 = 0, r2 = 0, r3_imm = 0 in
def FLUSH : YASarchInst<0xFF, (outs), (ins), "FLUSH", [(int_YASarch_flush)]>;

// delay
let r2 = 0, r3_imm = 0 in
def DELAY : YASarchInst<0xDB, (outs), (ins GPR:$r1), "DELAY $r1",
                        [(int_YASarch_delay GPR:$r1)]>;

// check finish
let r2 = 0, r3_imm = 0 in
def CHECKFINISH : YASarchInst<0xDA, (outs GPR:$r1), (ins),
                              "CHECKFINISH $r1",
                              [(set GPR:$r1, (int_YASarch_checkfinish))]>;

// ticks
let r2 = 0, r3_imm = 0 in
def GETTICKS : YASarchInst<0xDC, (outs GPR:$r1), (ins), "GETTICKS $r1",
                           [(set GPR:$r1, (int_YASarch_getticks))]>;

// rand
let r2 = 0, r3_imm = 0 in
def RAND : YASarchInst<0xDD, (outs GPR:$r1), (ins), "RAND $r1",
                       [(set GPR:$r1, (int_YASarch_rand))]>;

// putpixel
def PUTPIXEL : YASarchInst<0xEE, (outs), (ins GPR:$r1, GPR:$r2, GPR:$r3_imm),
                           "PUTPIXEL $r1 $r2 $r3_imm",
                           [(int_YASarch_putpixel GPR:$r1, GPR:$r2,
                                                 GPR:$r3_imm)]>;

//===----------------------------------------------------------------------===//
// YASarch Other patterns
//===----------------------------------------------------------------------===//

def i32hi16 : Operand<i32>, PatLeaf<(i32 imm), [{
    return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], HI16> {
}

def : Pat<(i32 imm:$r3_imm),
          (ORI (MOVHI (HI16 imm:$r3_imm)), (LI16 imm:$r3_imm))>;
def : Pat<(i32 i32hi16:$r3_imm), (MOVHI i32hi16:$r3_imm)>;
