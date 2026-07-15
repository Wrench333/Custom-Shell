#include "CdCommand.hpp"

#include <filesystem>
#include <iostream>

void CdCommand::execute(const std::vector<std::string> &args) {
  std::string npath = args[0];
  if (std::filesystem::exists(npath)) {
    std::filesystem::current_path(npath);
  } else if (npath == "~") {
    std::filesystem::current_path(std::getenv("HOME"));
  } else {
    std::cout << "cd: " << npath << ": No such file or directory\n";
  }
}
