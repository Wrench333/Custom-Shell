#pragma once

#include "Command.hpp"

class CdCommand : public Command {
public:
  void execute(const std::vector<std::string> &args) override;
};
