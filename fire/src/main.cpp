#include <fire/logger.hpp>
#include <fire/parser.hpp>
#include <fire/platform/linux/watcher.hpp>
#include <fire/reloader.hpp>
#include <iostream>

int main() {
  std::shared_ptr<fire::Parser> parser = std::make_shared<fire::Parser>();
  std::string path = std::filesystem::current_path().string() + "/" + fire::Parser::get<std::string>("root", ".");
  fire::Logger logger(fire::Logger::Application::MAIN);
  logger.log("Starting fire...🔥");
#ifdef __linux__
  fire::Watcher watcher(path);
  watcher.watch();
#endif

  return 0;
}
