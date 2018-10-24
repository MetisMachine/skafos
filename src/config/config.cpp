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

