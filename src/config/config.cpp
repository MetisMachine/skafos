#include "config.h"

// Helpers

bool is_all_scalar(YAML::Node value) {
  bool all_scalar;
  all_scalar = true;
  for(unsigned i=0; i<value.size(); i++) {
    YAML::Node sequence_item= value[i];
    if (sequence_item.IsScalar()){
      all_scalar = all_scalar && true;
    } else{
      all_scalar = all_scalar && false;
    }
  }
  return all_scalar;
}

bool is_number(std::string s){
  bool is_num;
  is_num = true;
  for (int i = 0; i < s.size(); i++){
    if (std::isdigit(s[i])){
      is_num = is_num && true;
    } else{
      is_num = is_num && false;
    }
  }
  return is_num;
}

bool is_bool(std::string s){

  for (int i = 0; i < s.size(); i++)
    s[i] = tolower(s[i]);
  if (s.compare("true") == 0 || s.compare("false") == 0) {
    return true;
  } else {
    return false;
  }
}

bool convert_to_bool(std::string s){
  for (int i = 0; i < s.size(); i++)
    s[i] = tolower(s[i]);
  if (s.compare("true") == 0) {
    return true;
  } else {
    return false;
  }
}

// LOAD CONFIG

// catch(YAML::ParserException& e)
YAML::Node Config::load_file(std::string file_path) {
  YAML::Node config;
  config = YAML::LoadFile(file_path);
  return config;
}

// catch(YAML::ParserException& e)
YAML::Node Config::load_json(Json json) {
  YAML::Node config;
  std::string string_json = json.dump();
  config = YAML::Load(string_json);
  return config;
}

void Config::yaml_to_file(YAML::Node node, std::string output_path) {
  std::ofstream fout(output_path);
  fout << node;
}

YAML::Node Config::edit(std::string file_path) {
  console::info("Opening editable config file...");
  std::string cmd = "vi " + file_path;
  int result = system(cmd.c_str());
  
  YAML::Node edited_yaml; 
  edited_yaml = load_file(file_path);
  return edited_yaml;
}


// JSON TO YAML

YAML::Node Config::to_yaml_block(YAML::Node node) {
  YAML::Node builder;
  for(YAML::const_iterator it = node.begin(); it != node.end(); ++it){
    YAML::Node nested;
    std::string key = it->first.as<std::string>();
    YAML::Node value = it->second;

    switch (value.Type()) {
      case YAML::NodeType::Scalar:
        value.SetStyle(YAML::EmitterStyle::Block);
        builder[key] = value;
        break;
      case YAML::NodeType::Sequence:
        builder = sequence_to_yaml_block(builder, value, key);
        break;
      case YAML::NodeType::Map:
        builder = map_to_yaml_block(builder, value, key);
        break;
      case YAML::NodeType::Undefined:
          // catch (const char* msg)
          throw "Unable to parse undefined YAML Node";
        break;
      case YAML::NodeType::Null:
        builder[key] = value;
    }
  }
  return builder;
}

YAML::Node Config::nested_yaml(YAML::Node value) {
  YAML::Node builder;
  YAML::Node nested_seq;
  if (value.IsSequence()){
    return sequence_to_yaml_block(builder, value);
  } else if (value.IsScalar()){
    value.SetStyle(YAML::EmitterStyle::Block);
    return value;
  } else {
    for(YAML::const_iterator it = value.begin(); it != value.end(); ++it){
      YAML::Node nested;
      YAML::Node double_nested;
      std::string key = it->first.as<std::string>();
      YAML::Node nested_value = it->second;

      switch (nested_value.Type()) {
        case YAML::NodeType::Scalar:
          nested_value.SetStyle(YAML::EmitterStyle::Block);
          builder[key] = nested_value;
          break;
        case YAML::NodeType::Sequence:
          builder = sequence_to_yaml_block(builder, nested_value, key);
          break;
        case YAML::NodeType::Map:
          builder = map_to_yaml_block(builder, nested_value, key);
          break;
        case YAML::NodeType::Undefined:
          throw "Unable to parse undefined YAML Node";
          break;
        case YAML::NodeType::Null:
          builder[key] = nested_value;
      }
    }
  }
  return builder;
}

YAML::Node Config::sequence_to_yaml_block(YAML::Node builder, YAML::Node value, std::string key){
  YAML::Node nested;
  bool all_scalar;
  all_scalar = is_all_scalar(value);
  if (not(all_scalar)) {
    for(unsigned int j=0; j<value.size(); j++){
      nested = Config::nested_yaml(value[j]);
      nested.SetStyle(YAML::EmitterStyle::Block);
      builder[key][j] = nested;
    }
  } else {
    value.SetStyle(YAML::EmitterStyle::Block);
    builder[key] = value;
  }
  return builder;
}

YAML::Node Config::sequence_to_yaml_block(YAML::Node builder, YAML::Node value){
  YAML::Node nested;
  bool all_scalar;
  all_scalar = is_all_scalar(value);
  if (not(all_scalar)) {
    for(unsigned int j=0; j<value.size(); j++){
      nested = nested_yaml(value[j]);
      nested.SetStyle(YAML::EmitterStyle::Block);
      builder[j] = nested;
    }
    return builder;
  } else {
    value.SetStyle(YAML::EmitterStyle::Block);
    return value;
  }
}

YAML::Node Config::map_to_yaml_block(YAML::Node builder, YAML::Node value, std::string key){
  YAML::Node nested;
  nested = nested_yaml(value);
  nested.SetStyle(YAML::EmitterStyle::Block);
  builder[key] = nested;
  return builder;
}

// YAML TO JSON

Json::object Config::yaml_to_json(YAML::Node node) {
  Json::object builder;
  for(YAML::const_iterator it = node.begin(); it != node.end(); ++it){
    Json::array to_list;
    Json::object nested_object;
    YAML::Node nested;
    std::string key = it->first.as<std::string>();
    YAML::Node value = it->second;
    bool all_scalar = true;

    switch (value.Type()) {
      case YAML::NodeType::Scalar:
        builder = handle_scalar_types(builder, value, key);
        break;
      case YAML::NodeType::Sequence:
        to_list = nested_sequence(value);
        builder[key] = to_list;
        break;
      case YAML::NodeType::Map:
        nested_object = nested_json(value);
        builder[key] = nested_object;
        break;
      case YAML::NodeType::Undefined:
        throw "Unable to parse undefined YAML Node";
        break;
      case YAML::NodeType::Null:
        builder[key] = nullptr;
    }
  }
  return builder;
}

Json::object Config::handle_scalar_types(Json::object builder, YAML::Node value, std::string key){
  if (is_number(value.as<std::string>())) {
    builder[key] = value.as<int>();
  } else if (is_bool(value.as<std::string>())){
    bool value_bool = convert_to_bool(value.as<std::string>());
    builder[key] = value_bool;
  } else {
    builder[key] = value.as<std::string>();
  }
  return builder;
}

Json::object Config::nested_json(YAML::Node value) {
  Json::object builder;
  for(YAML::const_iterator it = value.begin(); it != value.end(); ++it){
    Json::object nested_object;
    Json::array to_list;
    Json::array nested_list;
    YAML::Node nested;
    YAML::Node double_nested;
    bool all_scalar = true;
    std::string key = it->first.as<std::string>();
    YAML::Node nested_value = it->second;
    all_scalar = true;
    switch (nested_value.Type()) {
    case YAML::NodeType::Scalar:
      builder = handle_scalar_types(builder, nested_value, key);
      break;
    case YAML::NodeType::Sequence:
      to_list = nested_sequence(nested_value);
      builder[key] = to_list;
      break;
    case YAML::NodeType::Map:
      nested_object = nested_json(nested_value);
      builder[key] = nested_object;
      break;
    case YAML::NodeType::Undefined:
      throw "Unable to parse undefined YAML Node";
      break;
    case YAML::NodeType::Null:
      builder[key] = nullptr;
    }
  }
  return builder;
}

Json::array Config::nested_sequence(YAML::Node value) {
  Json::array to_list;
  bool all_scalar;
  all_scalar = is_all_scalar(value);

  if (not(all_scalar)) {
    for(unsigned int j=0; j<value.size(); j++){
      Json::object nested_object;
      Json::array nested_list;
      if (value[j].IsSequence()){
        nested_list = nested_sequence(value[j]);
        to_list.push_back(nested_list);
      } else if(value[j].IsScalar()) {
        if (is_number(value[j].as<std::string>())) {
          to_list.push_back(value[j].as<int>());
        } else if (is_bool(value[j].as<std::string>())){
          bool nested_bool = convert_to_bool(value[j].as<std::string>());
          to_list.push_back(nested_bool);
        } else {
          to_list.push_back(value[j].as<std::string>());
        }
      } else {
        nested_object = nested_json(value[j]);
        to_list.push_back(nested_object);
      }
    }
  } else {
    for(unsigned int j=0; j<value.size(); j++){
      YAML::Node one_item = value[j];
      if (is_number(one_item.as<std::string>())) {
        to_list.push_back(one_item.as<int>());
      } else if (is_bool(one_item.as<std::string>())){
        bool value_bool = convert_to_bool(one_item.as<std::string>());
        to_list.push_back(value_bool);
      } else {
        to_list.push_back(one_item.as<std::string>());
      }
    }
  }
  return to_list;
}
