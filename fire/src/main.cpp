#include <fire/parser.hpp>
#include <fire/platform/linux.hpp>
#include <fire/watch.hpp>
#include <iostream>

int main() {
  fire::Parser parser = fire::Parser();
  auto ans = parser.get<std::string>("bin", "", "build");
  std::cout << ans << std::endl;
  auto jet = parser.get_array<std::string>("post_cmd", "build");
  for (auto& item : *jet) {
    std::cout << item << std::endl;
  }
  std::string path = "/home/akshit/Projects/coding-cpp/fire";  // Change to your directory
  InotifyWatcher watcher(path);
  watcher.run();

  return 0;

  return 0;
}
