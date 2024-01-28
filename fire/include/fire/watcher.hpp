#pragma once

#include <dirent.h>
#include <fire/parser.hpp>
#include <queue>
#include <string>

namespace fire {
class Watcher {
protected:
  static bool isValidFile(const std::string& filename);
  static bool isValidDirectory(const std::string& path);
  void addWatchRecursively(const std::string& path);

  virtual void addWatch(const std::string& path) = 0;

public:
  virtual void watch() = 0;
  Watcher();
  ~Watcher();
};

}  // namespace fire
