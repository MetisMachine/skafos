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

int Config::edit(std::string file_path) {
  std::string cmd = "vi " + file_path;
  int result = system(cmd.c_str());
  return result;
  // TODO: load file as yaml node instead
  // string res = FileManager::read(filename);
}


// JSON TO YAML

YAML::Node Config::to_yaml_block(YAML::Node node) {
  YAML::Node builder;
  YAML::Node nested;
  for(YAML::const_iterator it = node.begin(); it != node.end(); ++it){
    std::string key = it->first.as<std::string>();
    YAML::Node value = it->second;
    cout<< "key: " << key << endl;
    cout << "value: " << value << endl;
    cout << endl;
    bool all_scalar = true;

    switch (value.Type()) {
      case YAML::NodeType::Scalar:
        value.SetStyle(YAML::EmitterStyle::Block);
        builder[key] = value;
        break;
      case YAML::NodeType::Sequence:
        for(unsigned i=0; i<value.size(); i++) {
          YAML::Node sequence_item= value[i];
          if (sequence_item.IsScalar()){
            all_scalar = all_scalar && true;
          } else{
            all_scalar = all_scalar && false;
          }
        }
        if (not(all_scalar)) {
          cout << "not all items are scalars" << endl;
          for(unsigned int j=0; j<value.size(); j++){
            nested = nested_yaml(value[j]);
            nested.SetStyle(YAML::EmitterStyle::Block);
            builder[key][j] = nested;
          }
        } else {
          value.SetStyle(YAML::EmitterStyle::Block);
          builder[key] = value;
        }
        break;
      case YAML::NodeType::Map:
        nested = nested_yaml(value);
        nested.SetStyle(YAML::EmitterStyle::Block);
        builder[key] = nested;
        break;
      // TO DO: Handle nulls and invalids better
      case YAML::NodeType::Undefined:
          cout << "undefined" << endl;
        break;
      case YAML::NodeType::Null:
        cout << "null" << endl;
    }
  }
  return builder;
}

