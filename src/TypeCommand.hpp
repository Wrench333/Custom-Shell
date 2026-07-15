#pragma once

#include <map>
#include <memory>
#include <string>

#include "Command.hpp"

class TypeCommand : public Command {
public:
  explicit TypeCommand(const std::map<std::string, std::unique_ptr<Command>> &builtins)
      : builtins_(builtins) {}

  void execute(const std::vector<std::string> &args) override;

private:
  const std::map<std::string, std::unique_ptr<Command>> &builtins_;
};
