add_llvm_component_library(LLVMYASarchInfo
  YASarchTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  YASarch
  )