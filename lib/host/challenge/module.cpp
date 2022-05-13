#include "host/challenge/module.h"
#include "host/challenge/func.h"

namespace WasmEdge {
namespace Host {

/// Register your functions in module.
HostFuncChallengeModule::HostFuncChallengeModule()
    : ModuleInstance("host_function_challenge") {
  addHostFunc("host_function_challenge_set_class_id",
              std::make_unique<HostFuncChallengeSetClassID>(Env));
  addHostFunc("host_function_challenge_add_student",
              std::make_unique<HostFuncChallengeAddStudent>(Env));
  addHostFunc("host_function_challenge_set_class_name",
              std::make_unique<HostFuncChallengeSetClassName>(Env));
  addHostFunc("host_function_challenge_print",
              std::make_unique<HostFuncChallengePrint>(Env));
  addHostFunc("host_function_challenge_cowsay",
              std::make_unique<HostFuncChallengeCowsay>(Env));
}

} // namespace Host
} // namespace WasmEdge