#include "parser.hpp"
#include <stdexcept>

namespace config_file
{

parse_fn get_parser(std::filesystem::path const & config_file)
{
    auto const extension = config_file.extension();
    if (extension == ".xml")
    {
        return parse_xml;
    }
    
    if (extension == ".json")
    {
        return parse_json;
    }

    if ((extension == ".yml") || (extension == ".yaml"))
    {
        return parse_yaml;
    }

    if (extension == ".toml")
    {
        return parse_toml;
    }

    throw std::runtime_error("unknown config file format");
}

}