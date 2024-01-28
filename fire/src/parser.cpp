#include <fire/parser.hpp>

namespace fire {

Table Parser::config = nullptr;

Parser::Parser()
    : Parser(std::filesystem::current_path().string() + "/.fireconfig.toml") {
  std::cout << "Using default config file: " << std::filesystem::current_path().string() + "/.fireconfig.toml" << std::endl;
}

Parser::Parser(const std::string& path) {
  try {
    this->config = cpptoml::parse_file(path);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}

Parser::~Parser() {
}

}  // namespace fire