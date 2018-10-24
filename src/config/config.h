#ifndef __CLI_CONFIG__
#define __CLI_CONFIG__

#include "common.h"
#include "yaml-cpp/yaml.h"

using namespace json11;

class Config {
  public: 
    static YAML::Node to_yaml_block(YAML::Node node);
    static Json::object yaml_to_json(YAML::Node node);
    static void yaml_to_file(YAML::Node node, std::string output_path);
    static YAML::Node load_file(std::string file_path);
    static YAML::Node load_json(Json json);
    static int edit(std::string file_path);
  private:
    static YAML::Node nested_yaml(YAML::Node value);
    static Json::object nested_json(YAML::Node value);
    static Json::array nested_sequence(YAML::Node value);
};

#endif