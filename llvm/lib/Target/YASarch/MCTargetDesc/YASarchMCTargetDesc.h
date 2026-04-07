#pragma once

namespace llvm {
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;

MCCodeEmitter *createYASarchMCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx);
} // namespace llvm

// Defines symbolic names for YASarch registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "YASarchGenRegisterInfo.inc"

// Defines symbolic names for the YASarch instructions.
#define GET_INSTRINFO_ENUM
#include "YASarchGenInstrInfo.inc"
