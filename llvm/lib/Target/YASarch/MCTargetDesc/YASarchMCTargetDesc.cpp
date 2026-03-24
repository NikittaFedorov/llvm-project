#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCDwarf.h"

#include "llvm/Support/ErrorHandling.h"

#include "TargetInfo/YASarchTargetInfo.h"
#include "MCTargetDesc/YASarchInfo.h"
#include "YASarchMCAsmInfo.h"

#include "YASarch.h"
#include "YASarchInstrPrinter.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "YASarchGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "YASarchGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "YASarchGenSubtargetInfo.inc"

static MCRegisterInfo *createYASarchMCRegisterInfo(const Triple &TT) {
  YASarch_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitYASarchMCRegisterInfo(X, YASarch::R0);


  return X;
}

static MCInstrInfo *createYASarchMCInstrInfo() {
  YASarch_DUMP_MAGENTA
  MCInstrInfo *X = new MCInstrInfo();
  InitYASarchMCInstrInfo(X);
  return X;
}

static MCSubtargetInfo *createYASarchMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  YASarch_DUMP_MAGENTA
  return createYASarchMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCAsmInfo *createYASarchMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  YASarch_DUMP_MAGENTA
  MCAsmInfo *MAI = new YASarchELFMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(YASarch::R1, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createYASarchMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  YASarch_DUMP_MAGENTA
  return new YASarchInstPrinter(MAI, MII, MRI);
}


// We need to define this function
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeYASarchTargetMC() {
    YASarch_DUMP_MAGENTA
    Target &TheYASarchTarget = getTheYASarchTarget();
    RegisterMCAsmInfoFn X(TheYASarchTarget, createYASarchMCAsmInfo);
    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(TheYASarchTarget, createYASarchMCRegisterInfo);
    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(TheYASarchTarget, createYASarchMCInstrInfo);
    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(TheYASarchTarget,
                                          createYASarchMCSubtargetInfo);
    

    // Register the MCInstPrinter
    TargetRegistry::RegisterMCInstPrinter(TheYASarchTarget, createYASarchMCInstPrinter);
}