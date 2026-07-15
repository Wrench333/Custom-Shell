#include "PwdCommand.hpp"

#include <iostream>
#include <filesystem>

void PwdCommand::execute(const std::vector<std::string> &args) {
  std::string cwd = std::filesystem::current_path().string();
  std::cout << cwd << "\n";
}
