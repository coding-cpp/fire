#pragma once

#include <fire/parser.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace fire {

class Logger {
private:
  static std::unique_ptr<std::ofstream> log_file;
  const std::string apps[4] = {"main", "watcher", "build", "runner"};
  static bool isInitialized;

  std::string color;
  std::string app;

  void setColor(std::string color);
  std::string getCurrentTime();

public:
  enum Application { MAIN = 0, WATCHER = 1, BUILD = 2, RUNNER = 3 };

  Logger(Application app);
  ~Logger();
  void log(const std::string& message);
};

}  // namespace fire