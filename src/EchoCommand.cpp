#include "EchoCommand.hpp"

#include <iostream>

void EchoCommand::execute(const std::vector<std::string> &args) {
  for (size_t i = 0; i < args.size(); ++i) {
    std::cout << args[i] << (i + 1 < args.size() ? " " : "");
  }
  std::cout << "\n";
}
