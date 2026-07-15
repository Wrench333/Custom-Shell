#pragma once

#include <string>

// Splits PATH into directories and finds the first executable match.
class PathResolver {
public:
  static std::string find(const std::string &name);
};
