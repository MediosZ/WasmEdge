
#pragma once

#include "common/defines.h"
#include "host/challenge/base.h"
#include <iostream>

namespace WasmEdge {
namespace Host {

class HostFuncChallengeSetClassID
    : public HostFuncChallenge<HostFuncChallengeSetClassID> {
public:
  HostFuncChallengeSetClassID(HostFuncChallengeEnvironment &HostEnv)
      : HostFuncChallenge(HostEnv) {}
  Expect<void> body(Runtime::Instance::MemoryInstance *MemInst, uint32_t CID);
};

class HostFuncChallengeAddStudent
    : public HostFuncChallenge<HostFuncChallengeAddStudent> {
public:
  HostFuncChallengeAddStudent(HostFuncChallengeEnvironment &HostEnv)
      : HostFuncChallenge(HostEnv) {}
  Expect<uint32_t> body(Runtime::Instance::MemoryInstance *MemInst,
                        uint32_t StudentPtr, uint32_t StudentLen);
};

class HostFuncChallengeSetClassName
    : public HostFuncChallenge<HostFuncChallengeSetClassName> {
public:
  HostFuncChallengeSetClassName(HostFuncChallengeEnvironment &HostEnv)
      : HostFuncChallenge(HostEnv) {}
  Expect<void> body(Runtime::Instance::MemoryInstance *MemInst,
                    uint32_t ClassNamePtr, uint32_t ClassNameLen);
};

class HostFuncChallengePrint : public HostFuncChallenge<HostFuncChallengePrint> {
public:
  HostFuncChallengePrint(HostFuncChallengeEnvironment &HostEnv)
      : HostFuncChallenge(HostEnv) {}
  Expect<void> body(Runtime::Instance::MemoryInstance *MemInst);
};

} // namespace Host
} // namespace WasmEdge