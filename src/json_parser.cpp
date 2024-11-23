#include "parser.hpp"
#include "json_schema.hpp"

#include <nlohmann/json-schema.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

namespace config_file
{

config parse_json(std::filesystem::path const & config_file)
{
    std::ifstream in(config_file);
    json contents = json::parse(in);

    json_validator validator;
    validator.set_root_schema(json::parse(config_file::schema_json));
//
//    validator.validate(contents);

    config_file::config cfg;

    auto const & server = contents.at("server");
    server.at("port").get_to(cfg.port);
    server.at("document-root").get_to(cfg.document_root);

    auto const & mimetypes = contents.at("mimetypes");
    for(auto const & type: mimetypes) {
        config_file::mimetype t;
        type.at("extension").get_to(t.extension);
        type.at("type").get_to(t.type);
        cfg.mime_types.emplace_back(t);
    }

    auto const & log = contents.at("log");
    log.at("level").get_to(cfg.log_level);
    log.at("destination").get_to(cfg.log_destination);

    return cfg;
}

}
