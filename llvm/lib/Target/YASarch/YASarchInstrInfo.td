//===----------------------------------------------------------------------===//
// YASarch profiles and nodes
//===----------------------------------------------------------------------===//

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

// movli
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0 in
def MOVLI : YASarchInst<0xAA, (outs GPR:$r1), (ins simm16:$r3_imm),
                    "MOVli $r1 $r3_imm",
                    [(set GPR:$r1, simm16:$r3_imm)]>;

let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0 in
def MOVHI : YASarchInst<0xAF, (outs GPR:$r1), (ins simm16:$r3_imm),
                        "MOVhi $r1 $r3_imm", []>;

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

def : Pat<(i32 imm:$r3_imm),
          (ORI (MOVHI (HI16 imm:$r3_imm)), (LI16 imm:$r3_imm))>;
