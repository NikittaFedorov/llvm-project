add_llvm_component_library(LLVMYASarchDesc
    YASarchMCTargetDesc.cpp
    YASarchMCAsmInfo.cpp
    YASarchInstrPrinter.cpp
    YASarchMCCodeEmitter.cpp
    YASarchAsmBackend.cpp
    YASarchELFObjectWriter.cpp

    LINK_COMPONENTS
    MC
    Support
    YASarchInfo

    ADD_TO_COMPONENT
    YASarch
)
