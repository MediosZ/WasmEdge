
#include "host/challenge/func.h"

namespace WasmEdge {
namespace Host {

Expect<void> HostFuncChallengeSetClassID::body(
    [[maybe_unused]] Runtime::Instance::MemoryInstance *MemInst, uint32_t CID) {
  Env.ClassID = CID;
  return {};
}

Expect<uint32_t>
HostFuncChallengeAddStudent::body(Runtime::Instance::MemoryInstance *MemInst,
                                  uint32_t StudentPtr, uint32_t StudentLen) {
  // Check memory instance from module.
  if (MemInst == nullptr) {
    return Unexpect(ErrCode::ExecutionFailed);
  }

  char *Student = MemInst->getPointer<char *>(StudentPtr);
  std::string NewStudent;
  std::copy_n(Student, StudentLen, std::back_inserter(NewStudent));
  Env.Students.push_back(std::move(NewStudent));
  return Env.Students.size();
}

Expect<void>
HostFuncChallengeSetClassName::body(Runtime::Instance::MemoryInstance *MemInst,
                                    uint32_t ClassNamePtr,
                                    uint32_t ClassNameLen) {
  // Check memory instance from module.
  if (MemInst == nullptr) {
    return Unexpect(ErrCode::ExecutionFailed);
  }

  char *ClassName = MemInst->getPointer<char *>(ClassNamePtr);
  std::string NewClassName;
  std::copy_n(ClassName, ClassNameLen, std::back_inserter(NewClassName));
  Env.ClassName = std::move(NewClassName);
  return {};
}

Expect<void> HostFuncChallengePrint::body(
    [[maybe_unused]] Runtime::Instance::MemoryInstance *MemInst) {
  std::cout << "Class ID: " << Env.ClassID << std::endl;
  std::cout << "Class Name: " << Env.ClassName << std::endl;
  for (auto &Student : Env.Students) {
    std::cout << "Student: " << Student << std::endl;
  }
  return {};
}

std::vector<std::string_view> split(std::string_view text, char delim) {
  std::vector<std::string_view> s;

  size_t start = 0, length = 0;
  for (const auto c : text) {
    ++length;
    if (c == delim) {
      s.push_back(text.substr(start, length - 1));

      start += length;
      length = 0;

      continue;
    }
  }

  s.push_back(text.substr(start));

  return s;
}

size_t get_width(const std::vector<std::string_view> message) {
  size_t width = 0;

  for (const auto line : message)
    width = std::max(width, line.size());

  return width;
}

void print_line(std::ostream &os, std::string_view line, size_t width,
                char left, char right) {
  os << left << ' ' << line;

  for (size_t i = 0; i < width - line.size() + 1; ++i)
    os << ' ';

  os << right << '\n';
}

void print_border(std::ostream &os, size_t width, char c) {
  os << ' ';

  for (size_t i = 0; i < width + 2; ++i)
    os << c;

  os << '\n';
}

void print_cow(std::ostream &os, const std::string_view content,
               char bubble_line) {
  auto lines = split(content, '\n');
  size_t i = 0;

  for (const auto line : lines) {
    switch (i) {
    case 0:
      os << '\t' << bubble_line << "   ";
      break;
    case 1:
      os << "\t " << bubble_line << "  ";
      break;
    default:
      os << "\t    ";
      break;
    }

    os << line << '\n';
    ++i;
  }
}

static std::string_view cow = 1 + R"(
^__^
(oo)\_______
(__)\       )\/\
	||----w |
	||     ||)";

std::string cowthink(const std::string_view text) {
  auto lines = WasmEdge::Host::split(text, '\n');
  auto width = WasmEdge::Host::get_width(lines);
  std::ostringstream oss;

  WasmEdge::Host::print_border(oss, width, '_');

  for (const auto line : lines)
    WasmEdge::Host::print_line(oss, line, width, '(', ')');

  WasmEdge::Host::print_border(oss, width, '-');
  WasmEdge::Host::print_cow(oss, WasmEdge::Host::cow, 'o');

  return oss.str();
}

Expect<void> HostFuncChallengeCowsay::body(
    [[maybe_unused]] Runtime::Instance::MemoryInstance *MemInst,
    uint32_t SentencePtr, uint32_t SentenceLen) {
  if (MemInst == nullptr) {
    return Unexpect(ErrCode::ExecutionFailed);
  }

  char *Sentence = MemInst->getPointer<char *>(SentencePtr);
  std::string NewSentence;
  std::copy_n(Sentence, SentenceLen, std::back_inserter(NewSentence));
  std::cout << "The cow is thinking about: " << NewSentence << std::endl;
  std::string cowmsg = WasmEdge::Host::cowthink(NewSentence);
  std::cout << cowmsg << std::endl;
  return {};
}

} // namespace Host
} // namespace WasmEdge