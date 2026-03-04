#ifndef LLVM_LIB_TARGET_YASarch_Sim_H
#define LLVM_LIB_TARGET_YASarch_Sim_H

#include "llvm/Support/raw_ostream.h"

#define YASarch_DUMP(Color)                                                        \
  {                                                                            \
    llvm::errs().changeColor(Color)                                            \
        << __func__ << "\n\t\t" << __FILE__ << ":" << __LINE__ << "\n";        \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  }

#define YASarch_DUMP_RED YASarch_DUMP(llvm::raw_ostream::RED)
#define YASarch_DUMP_GREEN YASarch_DUMP(llvm::raw_ostream::GREEN)
#define YASarch_DUMP_YELLOW YASarch_DUMP(llvm::raw_ostream::YELLOW)
#define YASarch_DUMP_CYAN YASarch_DUMP(llvm::raw_ostream::CYAN)
#define YASarch_DUMP_MAGENTA YASarch_DUMP(llvm::raw_ostream::MAGENTA)

#endif // LLVM_LIB_TARGET_YASarch_Sim_H