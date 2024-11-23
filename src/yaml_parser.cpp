#include "parser.hpp"
#include <yaml-cpp/yaml.h>

#include <stdexcept>

namespace config_file
{

config parse_yaml(std::filesystem::path const & config_file)
{
    config cfg;
    auto yaml = YAML::LoadFile(config_file);

    if (!yaml["server"])
    {
        throw std::runtime_error("missing required component \'server\'");
    }    
    auto server = yaml["server"];
    cfg.port = server["port"].as<int>();
    cfg.document_root = server["document-root"].as<std::string>();

    if (!yaml["mimetypes"])
    {
        throw std::runtime_error("missing required component \'log\'");
    }    
    auto mimetypes = yaml["mimetypes"];
    for(auto const & type: mimetypes) {
        mimetype t;
        t.extension = type["extension"].as<std::string>();
        t.type = type["type"].as<std::string>();

        cfg.mime_types.emplace_back(t);
    }


    if (!yaml["log"])
    {
        throw std::runtime_error("missing required component \'log\'");
    }    
    auto log = yaml["log"];
    cfg.log_level = log["level"].as<std::string>();
    cfg.log_destination = log["destination"].as<std::string>();


    return cfg;
}

}
