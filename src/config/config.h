#ifndef __CLI_CONFIG__
#define __CLI_CONFIG__

#include "common.h"
#include "yaml-cpp/yaml.h"

using namespace json11;
using namespace std;

class Config {
  public: 
    static YAML::Node to_yaml_block(YAML::Node node);
    static Json::object yaml_to_json(YAML::Node node);
    static void yaml_to_file(YAML::Node node, std::string output_path);
    static YAML::Node load_file(std::string file_path);
    static YAML::Node load_json(Json json);
    static YAML::Node edit(std::string file_path);
  private:
    static YAML::Node nested_yaml(YAML::Node value);
    static Json::object nested_json(YAML::Node value);
    static Json::object handle_scalar_types(Json::object builder, YAML::Node value, std::string key);
    static Json::array nested_sequence(YAML::Node value);
    static YAML::Node sequence_to_yaml_block(YAML::Node builder, YAML::Node value, std::string key);
    static YAML::Node sequence_to_yaml_block(YAML::Node builder, YAML::Node value);
    static YAML::Node map_to_yaml_block(YAML::Node builder, YAML::Node value, std::string key);
};

#endif