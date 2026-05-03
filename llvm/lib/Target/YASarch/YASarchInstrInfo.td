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

def simm16 : Operand<i32>, ImmLeaf<i32, [{return isUInt<16>(Imm);}]> {
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

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def INC_EQi : YASarchInst<0x90, (outs GPR:$r1, GPR:$r2_), (ins GPR:$r2, simm16:$r3_imm),
                          "INC_EQi $r1 $r2 $r3_imm", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def INC_NEi : YASarchInst<0x91, (outs GPR:$r1, GPR:$r2_), (ins GPR:$r2, simm16:$r3_imm),
                          "INC_NEi $r1 $r2 $r3_imm", []>;

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

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def ADD : YASarchInst<0x30, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                      "ADD $r1 $r2 $r3_imm",
                      [(set GPR:$r1, (add GPR:$r2, GPR:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def MUL : YASarchInst<0x32, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                      "MUL $r1 $r2 $r3_imm",
                      [(set GPR:$r1, (mul GPR:$r2, GPR:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def ORI : YASarchInst<0x44, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
                      "ORi $r1 $r2 $r3_imm",
                      [(set GPR:$r1, (or GPR:$r2, simm16:$r3_imm))]>;

// flush
let r1 = 0, r2 = 0, r3_imm = 0 in
def FLUSH : YASarchInst<0xFF, (outs), (ins), "FLUSH", [(int_YASarch_flush)]>;

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
