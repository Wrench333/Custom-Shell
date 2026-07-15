#include "TypeCommand.hpp"

#include <iostream>

#include "PathResolver.hpp"

void TypeCommand::execute(const std::vector<std::string> &args) {
  if (args.empty()) return;
  const std::string &name = args[0];

  if (builtins_.count(name)) {
    std::cout << name << " is a shell builtin\n";
    return;
  }

  std::string path = PathResolver::find(name);
  if (!path.empty()) {
    std::cout << name << " is " << path << "\n";
  } else {
    std::cout << name << ": not found\n";
  }
}
