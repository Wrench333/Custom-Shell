#pragma once

#include "Command.hpp"

class EchoCommand : public Command {
public:
  void execute(const std::vector<std::string> &args) override;
};
