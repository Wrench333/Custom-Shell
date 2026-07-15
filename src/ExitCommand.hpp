#pragma once

#include "Command.hpp"

class ExitCommand : public Command {
public:
  explicit ExitCommand(bool &running) : running_(running) {}
  void execute(const std::vector<std::string> &) override { running_ = false; }

private:
  bool &running_;
};
