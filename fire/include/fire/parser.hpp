#pragma once
#include <cpptoml.hpp>
#include <filesystem>
#include <iostream>

namespace fire {

typedef std::shared_ptr<cpptoml::table> Table;

class Parser {
private:
  static Table config;

public:
  Parser();
  Parser(const std::string& path);
  ~Parser();

  template<typename T>
  static T get(const std::string& key, const T& default_value, const std::string& toml_table = "") {
    if (toml_table != "") {
      auto table = config->get_table(toml_table);
      return table->get_as<T>(key).value_or(default_value);

    } else {
      return config->get_as<T>(key).value_or(default_value);
    }
  }
  template<typename T>
  static cpptoml::option<std::vector<T>> get_array(const std::string& key, const std::string& toml_table = "") {
    if (toml_table != "") {
      auto table = config->get_table(toml_table);
      return table->get_array_of<T>(key);
    } else {
      return config->get_array_of<T>(key);
    }
  };
};
}  // namespace fire