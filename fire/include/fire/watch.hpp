#pragma once

#include <string>

class Watcher {
public:
  virtual void watch(const std::string& directory) = 0;
  virtual void on_file_modified(const std::string& filename) = 0;
};
