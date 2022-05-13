#pragma once

#include "host/challenge/env.h"
#include "runtime/instance/module.h"

#include <cstdint>

namespace WasmEdge {
namespace Host {

class HostFuncChallengeModule : public Runtime::Instance::ModuleInstance {
public:
  HostFuncChallengeModule();

  HostFuncChallengeEnvironment &getEnv() { return Env; }
  const HostFuncChallengeEnvironment &getEnv() const noexcept { return Env; }

private:
  HostFuncChallengeEnvironment Env;
};

} // namespace Host
} // namespace WasmEdge