#pragma once

namespace llvm {
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCAsmBackend;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter *createYASarchMCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx);
MCAsmBackend *createYASarchAsmBackend(const Target &T,
                                      const MCSubtargetInfo &STI,
                                      const MCRegisterInfo &MRI,
                                      const MCTargetOptions &Options);
} // namespace llvm

// Defines symbolic names for YASarch registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "YASarchGenRegisterInfo.inc"

// Defines symbolic names for the YASarch instructions.
#define GET_INSTRINFO_ENUM
#include "YASarchGenInstrInfo.inc"
