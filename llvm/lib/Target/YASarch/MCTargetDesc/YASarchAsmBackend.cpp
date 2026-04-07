#include "MCTargetDesc/YASarchMCTargetDesc.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/EndianStream.h"

using namespace llvm;

namespace {
class YASarchAsmBackend : public MCAsmBackend {
protected:
  const Target &TheTarget;

public:
  YASarchAsmBackend(const Target &T)
      : MCAsmBackend(llvm::endianness::little), TheTarget(T) {}

  unsigned getNumFixupKinds() const override { return 0; }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override {
    // Cannot emit NOP with size not multiple of 32 bits.
    if (Count % 4 != 0)
      return false;

    uint64_t NumNops = Count / 4;
    for (uint64_t i = 0; i != NumNops; ++i)
      support::endian::write<uint32_t>(OS, 0x01000000, Endian);

    return true;
  }
};

class ELFYASarchAsmBackend : public YASarchAsmBackend {
  Triple::OSType OSType;

public:
  ELFYASarchAsmBackend(const Target &T, Triple::OSType OSType)
      : YASarchAsmBackend(T), OSType(OSType) {}

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override {
    return;
  }

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(OSType);
    return createYASarchELFObjectWriter(false, OSABI);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createYASarchAsmBackend(const Target &T,
                                        const MCSubtargetInfo &STI,
                                        const MCRegisterInfo &MRI,
                                        const MCTargetOptions &Options) {
  return new ELFYASarchAsmBackend(T, STI.getTargetTriple().getOS());
}
