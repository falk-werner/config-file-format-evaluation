#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

namespace config_file
{

struct mimetype
{
    std::string extension;
    std::string type;
};

struct config
{
    int port;
    std::string document_root;
    std::vector<mimetype> mime_types;
    std::string log_level;
    std::string log_destination;
};

}

#endif
