#include "PathResolver.hpp"

#include <cstdlib>
#include <sstream>
#include <unistd.h>

std::string PathResolver::find(const std::string &name) {
  const char *pathEnv = std::getenv("PATH");
  if (!pathEnv) return "";

  std::istringstream pathStream(pathEnv);
  std::string dir;
  while (std::getline(pathStream, dir, ':')) {
    std::string candidate = dir + '/' + name;
    if (access(candidate.c_str(), X_OK) == 0) {
      return candidate;
    }
  }
  return "";
}
