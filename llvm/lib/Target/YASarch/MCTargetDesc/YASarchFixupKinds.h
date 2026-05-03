#pragma once

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace YASarch {

enum Fixups {
  fixup_YASarch_PC16 = FirstTargetFixupKind,
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};

} // namespace YASarch
} // namespace llvm
