#include <fire/parser.hpp>
#include <fire/watch.hpp>
#include <iostream>

int main() {
  fire::Parser parser = fire::Parser();
  auto ans = parser.get<std::string>("root");
  // std::cout << ans << std::endl;
  // for (auto& item : ans) {
  //   std::cout << item << std::endl;
  // }
  return 0;
}