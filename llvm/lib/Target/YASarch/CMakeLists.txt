add_llvm_component_group(YASarch)


set(LLVM_TARGET_DEFINITIONS YASarch.td)

tablegen(LLVM YASarchGenRegisterInfo.inc -gen-register-info)

add_public_tablegen_target(YASarchCommonTableGen)

add_llvm_target(YASarchCodeGen
    YASarchTargetMachine.cpp
    YASarchRegisterInfo.cpp
    
    LINK_COMPONENTS
    CodeGen
    Core
    Support
    Target
    YASarchInfo


    ADD_TO_COMPONENT
    YASarch
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)