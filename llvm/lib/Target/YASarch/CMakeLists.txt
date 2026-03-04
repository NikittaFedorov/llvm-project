add_llvm_component_group(YASarch)

add_llvm_target(YASarchCodeGen
  YASarchTargetMachine.cpp

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