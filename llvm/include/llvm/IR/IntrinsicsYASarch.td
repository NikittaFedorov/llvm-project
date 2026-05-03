let TargetPrefix = "YASarch" in {  // All intrinsics start with "llvm.YASarch.".

 def int_YASarch_putpixel: ClangBuiltin<"YASarchPutPixel">,
   Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
             [IntrNoMem, IntrHasSideEffects]>;

 def int_YASarch_flush: ClangBuiltin<"YASarchFlush">,
   Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

 def int_YASarch_delay: ClangBuiltin<"YASarchDelay">,
   Intrinsic<[], [llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

 def int_YASarch_rand: ClangBuiltin<"YASarchRand">,
   Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrHasSideEffects]>;

 def int_YASarch_getticks: ClangBuiltin<"YASarchGetTicks">,
   Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrHasSideEffects]>;

 def int_YASarch_checkfinish: ClangBuiltin<"YASarchCheckFinish">,
   Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrHasSideEffects]>;

} // TargetPrefix = "YASarch"
