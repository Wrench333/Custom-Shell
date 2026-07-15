#pragma once

#include <string>
#include <vector>

// Every builtin implements this interface, so new builtins can be added
// without touching the dispatch logic (Open/Closed Principle).
class Command {
public:
  virtual ~Command() = default;
  virtual void execute(const std::vector<std::string> &args) = 0;
};
