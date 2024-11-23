#include "parser.hpp"

#include <toml.hpp>

#include <stdexcept>

namespace config_file
{

config parse_toml(std::filesystem::path const & config_file)
{
    config cfg;
    auto const toml = toml::parse(config_file);

    auto const & server = toml.at("server");
    cfg.port = server.at("port").as_integer();
    cfg.document_root = server.at("document_root").as_string();

    auto const & mimetypes = toml.at("mimetypes").as_array();
    for(auto const & type: mimetypes)
    {
        mimetype t;
        t.extension = type.at("extension").as_string();
        t.type = type.at("type").as_string();
        
        cfg.mime_types.emplace_back(t);
    }

    auto const & log = toml.at("log");
    cfg.log_level = log.at("level").as_string();
    cfg.log_destination = log.at("destination").as_string();

    return cfg;
}

}