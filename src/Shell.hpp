#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Command.hpp"

// Owns the builtin registry and the read-eval-print loop.
class Shell {
public:
  Shell();

  void run();

private:
  static std::vector<std::string> tokenize(const std::string &line);

  void dispatch(const std::vector<std::string> &tokens);

  // Strips a trailing ">"/"1>"/"2>" redirection from tokens, if present, and
  // returns the target file path (empty if there was no redirection).
  // targetFd is set to the fd being redirected (1 for ">"/"1>", 2 for "2>").
  static std::string extractOutputRedirect(std::vector<std::string> &tokens,
                                            int &targetFd);

  static void runExternal(const std::string &name, const std::string &path,
                           const std::vector<std::string> &args,
                           const std::string &outputFile, int targetFd);

  // execv needs a name-and-args array terminated by a null pointer.
  static std::vector<char *> buildArgv(const std::string &name,
                                        const std::vector<std::string> &args);

  bool running_;
  std::map<std::string, std::unique_ptr<Command>> builtins_;
};
