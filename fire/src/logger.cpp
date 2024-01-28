#include <fire/logger.hpp>

namespace fire {
bool Logger::isInitialized = false;
std::unique_ptr<std::ofstream> Logger::log_file = NULL;
// parser should already be initialized in main.cpp
Logger::Logger(Application app) {
  if (!Logger::isInitialized) {
    Logger::isInitialized = true;
    std::string configPath = Parser::get<std::string>("log", "", "build");
    if (configPath == "") {
      std::cerr << "No log file specified in .fireconfig.toml, logging to STDOUT" << std::endl;
    } else {
      configPath = fs::current_path().string() + '/' + configPath;
      Logger::log_file = std::make_unique<std::ofstream>(configPath);
    }
  }
  this->app = this->apps[app];
  const std::string& color = Parser::get<std::string>(this->app, "white", "color");
  this->setColor(color);
}
Logger::~Logger() {
  Logger::log_file->close();
}

void Logger::setColor(std::string color) {
  if (color == "red") {
    this->color = "\033[1;31m";
  } else if (color == "green") {
    this->color = "\033[1;32m";
  } else if (color == "yellow") {
    this->color = "\033[1;33m";
  } else if (color == "blue") {
    this->color = "\033[1;34m";
  } else if (color == "magenta") {
    this->color = "\033[1;35m";
  } else if (color == "cyan") {
    this->color = "\033[1;36m";
  } else if (color == "white") {
    this->color = "\033[1;37m";
  } else {
    this->color = "\033[0m";
  }
}
std::string Logger::getCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time_t);
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%d-%m-%Y %H:%M:%S");
  oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

  return oss.str();
}

void Logger::log(const std::string& message) {
  std::string time = "";
  std::string app = "[" + this->app + "] ";
  if (Parser::get<bool>("main_only", false, "log") && this->app != "main") {
    return;
  }

  if (Parser::get<bool>("time", false, "log")) {
    time = "[" + this->getCurrentTime() + "] ";
  }

  if (Logger::log_file == NULL) {
    std::cout << this->color << time << app << message << "\033[0m" << std::endl;
  } else {
    *Logger::log_file << time << app << message << std::endl;
  }
}
}  // namespace fire