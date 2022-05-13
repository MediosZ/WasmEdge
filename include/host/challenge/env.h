#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace WasmEdge {
namespace Host {

class HostFuncChallengeEnvironment {
public:
  /// Define your environment variables and data structures

  /// Const

  /// Variables
  uint32_t ClassID;
  std::string ClassName;
  std::vector<std::string> Students;

  /// Initial Configurations
};

} // namespace Host
} // namespace WasmEdge