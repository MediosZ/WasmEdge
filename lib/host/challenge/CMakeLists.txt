wasmedge_add_library(challenge
  func.cpp
  module.cpp
)

target_link_libraries(challenge
  PUBLIC
  wasmedgeCommon
  wasmedgeSystem
)