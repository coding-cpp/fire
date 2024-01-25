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
  T get(const std::string& key, const T& default_value, const std::string& toml_table = "") {
    if (toml_table != "") {
      auto table = this->config->get_table(toml_table);
      return table->get_as<T>(key).value_or(default_value);

    } else {
      return this->config->get_as<T>(key).value_or(default_value);
    }
  }
  template<typename T>
  cpptoml::option<std::vector<T>> get_array(const std::string& key, const std::string& toml_table = "") {
    if (toml_table != "") {
      auto table = this->config->get_table(toml_table);
      return table->get_array_of<T>(key);
    } else {
      return this->config->get_array_of<T>(key);
    }
  };
};
}  // namespace fire