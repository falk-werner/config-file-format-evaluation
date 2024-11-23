#include "parser.hpp"
#include "xml_schema.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

#include <cstring>

#include <string>
#include <sstream>
#include <stdexcept>

namespace
{

using xmlDoc_ptr = std::unique_ptr<xmlDoc, void(*)(xmlDoc*)>;
using xmlSchemaParserCtxt_ptr = std::unique_ptr<xmlSchemaParserCtxt, void(*)(xmlSchemaParserCtxt*)>;
using xmlSchema_ptr = std::unique_ptr<xmlSchema, void(*)(xmlSchema*)>;
using xmlSchemaValidCtxt_ptr = std::unique_ptr<xmlSchemaValidCtxt, void(*)(xmlSchemaValidCtxt*)>;

xmlNode * find_child(xmlNode* parent, std::string const & name)
{
    if (parent != nullptr)
    {
        for(xmlNode * current = parent->children; current != nullptr; current = current->next)
        {
            char const * current_name = reinterpret_cast<char const *>(current->name);
            if ((current->type == XML_ELEMENT_NODE) && (name == current_name))
            {
                return current;
            }
        }
    }

    return nullptr;
}

xmlNode * find_next(xmlNode* node, std::string const & name)
{
    while ((node != nullptr) && (node->next != nullptr))
    {
        node = node->next;
        char const * node_name = reinterpret_cast<char const *>(node->name);
        if ((node->type == XML_ELEMENT_NODE) && (name == node_name))
        {
            return node;
        }
    }

    return nullptr;
}

std::string get_content(xmlNode* node)
{
    auto * value = xmlNodeGetContent(node);
    if (nullptr == value)
    {
        return "";
    }

    std::string result(reinterpret_cast<char*>(value));
    xmlFree(value);

    return result;
}

std::string get_attribute(xmlNode * node, std::string const & name, std::string const & defaultValue)
{
    if (node == nullptr)
    {
        return defaultValue;
    }

    auto const * xml_name = reinterpret_cast<xmlChar const *>(name.c_str());
    auto * value = xmlGetProp(node, xml_name);
    if (value == nullptr)
    {
        return defaultValue;
    }

    std::string result(reinterpret_cast<char *>(value));
    xmlFree(value);

    return result;
}

bool is_valid(xmlDoc * doc, xmlDoc * schema_doc)
{
    auto * raw_schema_parser_context = xmlSchemaNewDocParserCtxt(schema_doc);
    if (raw_schema_parser_context == nullptr)
    {
        return false;
    }
    xmlSchemaParserCtxt_ptr schema_parser_context(raw_schema_parser_context, xmlSchemaFreeParserCtxt);

    auto * raw_schema = xmlSchemaParse(schema_parser_context.get());
    if (raw_schema == nullptr)
    {
        return false;
    }
    xmlSchema_ptr schema(raw_schema, xmlSchemaFree);


    auto * raw_validation_context = xmlSchemaNewValidCtxt(schema.get());
    if (raw_validation_context == nullptr)
    {
        return false;
    }
    xmlSchemaValidCtxt_ptr validation_context(raw_validation_context, xmlSchemaFreeValidCtxt);

    int const rc = xmlSchemaValidateDoc(validation_context.get(), doc);
    return (rc == 0);
}

int to_int(std::string const & value)
{
    size_t length;
    int const result = std::stoi(value, &length, 10);
    if (value.size() != length)
    {
        throw std::runtime_error("int value expected");
    }
    return result;
}

}


namespace config_file
{

config parse_xml(std::filesystem::path const & config_file)
{
    xmlDoc * raw_schema = xmlReadMemory(config_file::schema_xml, strlen(config_file::schema_xml), nullptr, nullptr, XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    if (nullptr == raw_schema)
    {
        throw std::runtime_error("failed to read schema");
    }
    xmlDoc_ptr schema(raw_schema, xmlFreeDoc);

    xmlDoc * raw_doc = xmlReadFile(config_file.string().c_str(), nullptr, XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    if (nullptr == raw_doc)
    {
        throw std::runtime_error("failed to read config file");
    }
    xmlDoc_ptr doc(raw_doc, xmlFreeDoc);

    bool const valid = is_valid(doc.get(), schema.get());
    if (!valid)
    {
        throw std::runtime_error("validation failed");
    }

    auto* root = xmlDocGetRootElement(doc.get());

    config cfg;

    auto * server = find_child(root, "server");
    cfg.port = to_int(get_content(find_child(server, "port")));
    cfg.document_root = get_content(find_child(server, "document-root"));

    auto * mimetypes = find_child(root, "mimetypes");
    auto * type = find_child(mimetypes, "mimetype");
    while (nullptr != type)
    {
        mimetype t;
        t.extension = get_attribute(type, "extension", "?");
        t.type = get_attribute(type, "value", "?");
        cfg.mime_types.emplace_back(t);

        type = find_next(type, "mimetype");
    }

    auto * log = find_child(root, "log");
    cfg.log_level = get_content(find_child(log, "level"));
    cfg.log_destination = get_content(find_child(log, "destination"));

    return cfg;
}
 
}
