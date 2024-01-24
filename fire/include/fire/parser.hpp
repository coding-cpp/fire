#include <cpptoml.hpp>
#include <filesystem>
#include <iostream>

namespace fire {

typedef std::shared_ptr<cpptoml::table> Table;
typedef cpptoml::option<std::vector<std::string>> StringArray;

class Parser {
private:
  Table config;
  void parse_table(Table table, const std::string& prefix);

public:
  Parser();
  Parser(const std::string& path);
  ~Parser();

  template<typename T>
  T get(const std::string& key) {
    return this->config->get_qualified_as<T>(key);
  }
};

}  // namespace fire