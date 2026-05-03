#pragma once

#include <memory>

namespace llvm {
class Triple;
class StringRef;
class MCAsmInfo;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCAsmBackend;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCRegisterInfo *createYASarchMCRegisterInfo(const Triple &TT);
MCInstrInfo *createYASarchMCInstrInfo();
MCSubtargetInfo *createYASarchMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                              StringRef FS);
MCAsmInfo *createYASarchMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                                  const MCTargetOptions &Options);
MCCodeEmitter *createYASarchMCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx);
MCAsmBackend *createYASarchAsmBackend(const Target &T,
                                      const MCSubtargetInfo &STI,
                                      const MCRegisterInfo &MRI,
                                      const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter>
createYASarchELFObjectWriter(bool Is64Bit, uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for YASarch registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "YASarchGenRegisterInfo.inc"

// Defines symbolic names for the YASarch instructions.
#define GET_INSTRINFO_ENUM
#include "YASarchGenInstrInfo.inc"
