cmake_minimum_required(VERSION 3.20)
project(config-file-format-evaluation)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)
pkg_check_modules(YamlCpp REQUIRED IMPORTED_TARGET yaml-cpp)

file(READ config/config.xsd CONFIG_SCHEMA_XML)
configure_file(src/xml_schema.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/xml_schema.hpp @ONLY)

file(READ config/config-schema.json CONFIG_SCHEMA_JSON)
configure_file(src/json_schema.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/json_schema.hpp @ONLY)

add_executable(read-config
    src/main.cpp
    src/parser.cpp
    src/xml_parser.cpp
    src/xml_schema.hpp
    src/json_parser.cpp
    src/json_schema.hpp
    src/yaml_parser.cpp
    src/toml_parser.cpp
)
target_include_directories(read-config PRIVATE src)
target_link_libraries(read-config PRIVATE 
    LibXml2::LibXml2
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
    PkgConfig::YamlCpp
)

