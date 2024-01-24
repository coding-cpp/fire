#include <fire/parser.hpp>

namespace fire {

Parser::Parser()
    : Parser(std::filesystem::current_path().string() + "/../.fireconfig.toml") {
  std::cout << "Using default config file: " << std::filesystem::current_path().string() + "/../.fireconfig.toml" << std::endl;
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

void Parser::parse_table(std::shared_ptr<cpptoml::table> table, const std::string& prefix = "") {
  for (auto& item : *table) {
    auto key = prefix + item.first;
    if (item.second->is_table()) {
      this->parse_table(item.second->as_table(), key + ".");
    } else {
      this->config->insert(key, item.second);
    }
  }
}

}  // namespace fire