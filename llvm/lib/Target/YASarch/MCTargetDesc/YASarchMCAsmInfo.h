#pragma once

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class YASarchELFMCAsmInfo : public MCAsmInfoELF {
public:
  explicit YASarchELFMCAsmInfo(const Triple &TheTriple);
};

} // end namespace llvm