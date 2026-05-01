#include "YASarch.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

void YASarchTargetInfo::getTargetDefines(const LangOptions &Opts,
                                         MacroBuilder &Builder) const {
  Builder.defineMacro("__YASarch__");
}

ArrayRef<Builtin::Info> YASarchTargetInfo::getTargetBuiltins() const {
  return std::nullopt;
}
