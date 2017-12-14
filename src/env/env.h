#ifndef __CLI_ENV__
#define __CLI_ENV__

#include "common.h"

const std::string METIS_API_TOKEN   = "METIS_API_TOKEN";
const std::string METIS_AUTH_TOKEN  = "METIS_OAUTH";
const std::string METIS_CREDENTIALS = "credentials.json";
const std::string METIS_TEMPLATE_REPO = "https://github.com/MetisMachine/templates.list.git";
struct metis_path {
  std::string home;
  std::string env;
  std::string credentials;
};

class Env {
public:
  metis_path paths;  
  static Env *instance();

  std::string get(std::string key);
  void set(std::string key, std::string val);
  void setup();
  bool load_credentials();
  void write_credentials(json11::Json object);

private:
  static Env *instance_;
  bool ping();
  
  Env();
  std::string home_dir();
  std::string metis_dir();
};

#endif
