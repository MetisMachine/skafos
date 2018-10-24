#include "config.h"

using namespace std;

// LOAD CONFIG

YAML::Node Config::load_file(std::string file_path) {
  YAML::Node config;
  try {
    config = YAML::LoadFile(file_path);
    return config;
  } catch(YAML::ParserException& e){
    cout << "could not load file" << endl;
  }
}

YAML::Node Config::load_json(Json json) {
  YAML::Node config;
  std::string string_json = json.dump();
  try {
    config = YAML::Load(string_json);
    return config;
  } catch(YAML::ParserException& e){
    cout << "could not load json" << endl;
  }
}

void Config::yaml_to_file(YAML::Node node, std::string output_path) {
  std::ofstream fout(output_path);
  fout << node;
}
