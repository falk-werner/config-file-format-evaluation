#ifndef PARSER_HPP
#define PARSER_HPP

#include "config.hpp"
#include <filesystem>

namespace config_file
{

using parse_fn = config (*)(std::filesystem::path const & config_file);

parse_fn get_parser(std::filesystem::path const & config_file);

config parse_xml(std::filesystem::path const & config_file);

config parse_json(std::filesystem::path const & config_file);

config parse_yaml(std::filesystem::path const & config_file);

config parse_toml(std::filesystem::path const & config_file);


}

#endif
