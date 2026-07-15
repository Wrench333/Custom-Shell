#pragma once

#include "Command.hpp"

class PwdCommand : public Command {
public:
  void execute(const std::vector<std::string> &args) override;
};
