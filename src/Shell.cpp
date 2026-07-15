#include "Shell.hpp"

#include <cctype>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

#include "CdCommand.hpp"
#include "EchoCommand.hpp"
#include "ExitCommand.hpp"
#include "PathResolver.hpp"
#include "PwdCommand.hpp"
#include "TypeCommand.hpp"

Shell::Shell() : running_(true) {
  builtins_["exit"] = std::make_unique<ExitCommand>(running_);
  builtins_["echo"] = std::make_unique<EchoCommand>();
  builtins_["type"] = std::make_unique<TypeCommand>(builtins_);
  builtins_["pwd"] = std::make_unique<PwdCommand>();
  builtins_["cd"] = std::make_unique<CdCommand>();
}

void Shell::run() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (running_) {
    std::cout << "$ ";
    std::string line;
    if (!std::getline(std::cin, line))
      break;

    std::vector<std::string> tokens = tokenize(line);
    if (tokens.empty())
      continue;

    dispatch(tokens);
  }
}

std::vector<std::string> Shell::tokenize(const std::string &line) {
  std::vector<std::string> tokens;
  std::string current;
  bool inSingleQuotes = false;
  bool inToken = false;
  char currQuotes;
  bool escaped = false;
  for (const char c : line) {
    if (escaped) {
      current += c;
      inToken = true;
      escaped = false;
      continue;
    }

    if (inSingleQuotes) {
      if (c == currQuotes) {
        inSingleQuotes = false;
      } else if (currQuotes == '\"' && c == '\\') {
        escaped = true;
      } else {
        current += c;
      }
      inToken = true;
      continue;
    }

    if (c == '\'') {
      inSingleQuotes = true;
      inToken = true;
      currQuotes = '\'';
      continue;
    }

    if (c == '\"') {
      inSingleQuotes = true;
      inToken = true;
      currQuotes = '\"';
      continue;
    }

    if (c == '\\') {
      escaped = true;
      continue;
    }

    if (c == ' ' && inToken) {
      tokens.push_back(current);
      current.clear();
      inToken = false;
      continue;
    }

    if (c != ' ') {
      current += c;
      inToken = true;
    }
  }

  if (inToken) {
    tokens.push_back(current);
  }

  return tokens;
}

std::string Shell::extractOutputRedirect(std::vector<std::string> &tokens,
                                         int &targetFd, bool &append) {
  for (size_t i = 0; i + 1 < tokens.size(); ++i) {
    if (tokens[i] == ">" || tokens[i] == "1>" || tokens[i] == "2>" || tokens[i] == "1>>" ||
        tokens[i] == ">>" || tokens[i] == "2>>") {
      targetFd = (tokens[i] == "2>" || tokens[i] == "2>>") ? STDERR_FILENO
                                                           : STDOUT_FILENO;
      append = (tokens[i] == ">>" || tokens[i] == "2>>" || tokens[i] == "1>>") ? true : false;
      std::string file = tokens[i + 1];
      tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
      return file;
    }
  }
  return "";
}

void Shell::dispatch(const std::vector<std::string> &tokens) {
  std::vector<std::string> mutableTokens = tokens;
  int targetFd = STDOUT_FILENO;
  bool append = false;
  std::string outputFile =
      extractOutputRedirect(mutableTokens, targetFd, append);

  const std::string &name = mutableTokens[0];
  std::vector<std::string> args(mutableTokens.begin() + 1, mutableTokens.end());

  auto it = builtins_.find(name);
  if (it != builtins_.end()) {
    int savedFd = -1;
    if (!outputFile.empty()) {
      std::cout.flush();
      std::cerr.flush();
      savedFd = dup(targetFd);
      int fd = open(outputFile.c_str(),
                    O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
      dup2(fd, targetFd);
      close(fd);
    }

    it->second->execute(args);

    if (savedFd != -1) {
      std::cout.flush();
      std::cerr.flush();
      dup2(savedFd, targetFd);
      close(savedFd);
    }
    return;
  }

  std::string path = PathResolver::find(name);
  if (path.empty()) {
    std::cout << name << ": command not found\n";
    return;
  }

  runExternal(name, path, args, outputFile, targetFd, append);
}

void Shell::runExternal(const std::string &name, const std::string &path,
                        const std::vector<std::string> &args,
                        const std::string &outputFile, int targetFd,
                        bool append) {
  std::vector<char *> argv = buildArgv(name, args);

  pid_t pid = fork();
  if (pid == 0) {
    // Child: replace this process image with the external program.
    if (!outputFile.empty()) {
      int fd = open(outputFile.c_str(),
                    O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
      dup2(fd, targetFd);
      close(fd);
    }
    execv(path.c_str(), argv.data());
    std::exit(127); // Only reached if execv fails.
  }

  // Parent: wait for the child to finish before showing the prompt again.
  int status;
  waitpid(pid, &status, 0);
}

std::vector<char *> Shell::buildArgv(const std::string &name,
                                     const std::vector<std::string> &args) {
  std::vector<char *> argv;
  argv.push_back(const_cast<char *>(name.c_str()));
  for (const auto &arg : args) {
    argv.push_back(const_cast<char *>(arg.c_str()));
  }
  argv.push_back(nullptr);
  return argv;
}
