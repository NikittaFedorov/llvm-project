#pragma once

#define GET_REGINFO_HEADER
#include "YASarchGenRegisterInfo.inc"

namespace llvm {

struct YASarchRegisterInfo : public YASarchGenRegisterInfo {
public:
  YASarchRegisterInfo();
};

} // end namespace llvm
