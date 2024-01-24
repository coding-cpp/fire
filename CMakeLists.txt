cmake_minimum_required(VERSION 3.15)

# Enables the Visibility Property on all target types
cmake_policy(SET CMP0063 NEW)

# Enables the new behavior of the if() command
cmake_policy(SET CMP0054 NEW)

project(fire LANGUAGES CXX DESCRIPTION "Hot reload package for expresso")
set(CMAKE_PROJECT_VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define installation folder and the module path (this one required by conan)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

add_subdirectory(fire)
