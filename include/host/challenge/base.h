#pragma once

#include "common/errcode.h"
#include "host/challenge/env.h"
#include "runtime/hostfunc.h"

namespace WasmEdge {
namespace Host {

template <typename T> class HostFuncChallenge : public Runtime::HostFunction<T> {
public:
  HostFuncChallenge(HostFuncChallengeEnvironment &HostEnv)
      : Runtime::HostFunction<T>(0), Env(HostEnv) {}

protected:
  HostFuncChallengeEnvironment &Env;
};

} // namespace Host
} // namespace WasmEdge