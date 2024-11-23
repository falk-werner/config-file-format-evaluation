# Config File Format Evaluation

This repo contains an evaluation of C++ libraries to parse config files in different formats.

## Challenge

The goal is to parse and validate a config file in the arbitraty format.
The config file should be parsed into the following C++ data class 
see [src/config.hpp](src/config.cpp):

```C++
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
```


## Comparison

| Category      | XML | JSON | YAML | TOML |
| ------------- | --- | ---- | ---- | ---- |
| Library | [libXml2](https://gitlab.gnome.org/GNOME/libxml2) | [nlohmann-json](https://github.com/nlohmann/json),  [json-schema-validator](https://github.com/pboettch/json-schema-validator) | [yaml-cpp](https://github.com/jbeder/yaml-cpp) | [toml11](https://github.com/ToruNiina/toml11) |
| OSS License | MIT | MIT | MIT | MIT |
| [OSF Score](https://github.com/ossf/scorecard) | [4.5](https://scorecard.dev/viewer/?uri=github.com/GNOME/libxml2) | [7.7 (nlohmann-json)](https://scorecard.dev/viewer/?uri=github.com/nlohmann/json), [4.8 (json-schema-validator)](https://scorecard.dev/viewer/?uri=github.com/pboettch/json-schema-validator) | [6.8](https://scorecard.dev/viewer/?uri=github.com/jbeder/yaml-cpp) | [5.2](https://scorecard.dev/viewer/?uri=github.com/ToruNiina/toml11) |
| Yocto layer | [openembedded-core](https://layers.openembedded.org/layerindex/branch/master/layer/openembedded-core/) | [meta-oe (nlohmann-json)](https://layers.openembedded.org/layerindex/branch/master/layer/meta-oe/), [meta-ros2-rolling (json-schema-validator)](https://layers.openembedded.org/layerindex/branch/master/layer/meta-ros2-rolling/) | [meta-oe](https://layers.openembedded.org/layerindex/branch/master/layer/meta-oe/) | [meta-oe](https://layers.openembedded.org/layerindex/branch/master/layer/meta-oe/) |
| Schema validation | yes | yes | no | no |
| Comments supported | yes | yes (non standard) | yes | yes |
| Lines of Code | [146](src/xml_parser.cpp) | [34](src/json_parser.cpp) | [37](src/yaml_parser.cpp) | [26](src/toml_parser.cpp) |
