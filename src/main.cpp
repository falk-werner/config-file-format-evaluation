#include "parser.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    if (argc > 1)
    {
        try
        {
            std::filesystem::path const config_file(argv[1]);
            auto parse = config_file::get_parser(config_file);
            auto config = parse(config_file);

            std::cout << "server:" << std::endl;
            std::cout << "\tport: " << config.port << std::endl;
            std::cout << "\tdocument-root: " << config.document_root << std::endl;
            std::cout << "mimetypes" << std::endl;
            for(auto const & type: config.mime_types) {
                std::cout << "\textension: " << type.extension << ", type: " << type.type << std::endl;

            }
            std::cout << "log:" << std::endl;
            std::cout << "\tlevel: " << config.log_level << std::endl;
            std::cout << "\tdestination: " << config.log_destination << std::endl; 
        }
        catch (std::exception const & ex)
        {
            exit_code = EXIT_FAILURE;
            std::cerr << "error: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "error: failed to parse config" << std::endl;
        }
    }
    else
    {
        std::cout << "usage: read-config <filename>" << std::endl;
    }

    return exit_code;
}