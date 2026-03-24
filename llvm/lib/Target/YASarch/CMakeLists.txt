add_llvm_component_group(YASarch)


set(LLVM_TARGET_DEFINITIONS YASarch.td)

tablegen(LLVM YASarchGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM YASarchGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM YASarchGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM YASarchGenMCPseudoLowering.inc -gen-pseudo-lowering)

add_public_tablegen_target(YASarchCommonTableGen)

add_llvm_target(YASarchCodeGen
    YASarchTargetMachine.cpp
    YASarchRegisterInfo.cpp
    YASarchInstrInfo.cpp
    YASarchSubtarget.cpp
    YASarchAsmPrinter.cpp
    
    LINK_COMPONENTS
    CodeGen
    Core
    Support
    Target
    YASarchInfo
    AsmPrinter


    ADD_TO_COMPONENT
    YASarch
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)