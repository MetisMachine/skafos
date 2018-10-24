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

YAML::Node Config::nested_yaml(YAML::Node value) {
  YAML::Node builder;
  YAML::Node nested;
  YAML::Node double_nested;
  bool all_scalar = true;
  if (value.IsSequence()){
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
        builder[j] = nested;
      }
    } else {
      value.SetStyle(YAML::EmitterStyle::Block);
      return value;
    }
  } else {
    for(YAML::const_iterator it = value.begin(); it != value.end(); ++it){
    std::string key = it->first.as<std::string>();
    YAML::Node nested_value = it->second;
    all_scalar = true;
    switch (nested_value.Type()) {
      case YAML::NodeType::Scalar:
        nested_value.SetStyle(YAML::EmitterStyle::Block);
        builder[key] = nested_value;
        break;
      case YAML::NodeType::Sequence:
        for(unsigned i=0; i<nested_value.size(); i++) {
          YAML::Node sequence_item= nested_value[i];
          if (sequence_item.IsScalar()){
            all_scalar = all_scalar && true;
          } else{
            all_scalar = all_scalar && false;
          }
        }
        if (not(all_scalar)) {
          for(unsigned int j=0; j<nested_value.size(); j++){
            double_nested = nested_yaml(nested_value[j]);
            double_nested.SetStyle(YAML::EmitterStyle::Block);
            builder[key][j] = double_nested;
          }
        } else {
          nested_value.SetStyle(YAML::EmitterStyle::Block);
          builder[key] = nested_value;
        }
        break;
      case YAML::NodeType::Map:
        nested = nested_yaml(nested_value);
        nested.SetStyle(YAML::EmitterStyle::Block);
        builder[key] = nested;
        break;
      case YAML::NodeType::Undefined:
        cout << "undefined" << endl;
        break;
      case YAML::NodeType::Null:
        cout << "null" << endl;
      }
    }
  }
  return builder;
}

// YAML TO JSON

bool is_number(const std::string& s){
            std::string::const_iterator it = s.begin();
            while (it != s.end() && std::isdigit(*it)) ++it;
            return !s.empty() && it == s.end();
        } 


Json::object Config::yaml_to_json(YAML::Node node) {
  Json::object builder;
  Json::object nested_object;
  Json::array nested_list;
  YAML::Node nested;
  Json::array to_list;
  for(YAML::const_iterator it = node.begin(); it != node.end(); ++it){
    std::string key = it->first.as<std::string>();
    YAML::Node value = it->second;
    cout<< "key: " << key << endl;
    cout << "value: " << value << endl;
    cout << endl;
    bool all_scalar = true;

    switch (value.Type()) {
      case YAML::NodeType::Scalar:
        cout << "value scalar: " << value << endl;
        // TODO: check if boolean
        if (is_number(value.as<std::string>())) {
          builder[key] = value.as<int>();
        }  else {
          builder[key] = value.as<std::string>();
        }
        break;
      case YAML::NodeType::Sequence:
        cout << "sequence" << endl;
        for(unsigned i=0; i<value.size(); i++) {
          YAML::Node sequence_item = value[i];
          if (sequence_item.IsScalar()){
            all_scalar = all_scalar && true;
          } else{
            all_scalar = all_scalar && false;
          }
        }
        if (not(all_scalar)) {
          cout << "not all items are scalars" << endl;
            for(unsigned int j=0; j<value.size(); j++){
              if (value[j].IsSequence()){
                nested_list = nested_sequence(value[j]);
                to_list.push_back(nested_list);
              } else {
                nested_object = nested_json(value[j]);
                to_list.push_back(nested_object);
              }
            }
        } else {
          for(unsigned int j=0; j<value.size(); j++){
            // nested = nested_yaml(value[j]);
            YAML::Node one_item = value[j];
            cout << "one_item: " << one_item << endl;
            // TODO: handle different value types like scalar switch statement
            to_list.push_back(one_item.as<std::string>());
          }
        }
        builder[key] = to_list;
        break;
      case YAML::NodeType::Map:
        cout << "map" << endl;
        nested_object = nested_json(value);
        builder[key] = nested_object;
        break;
