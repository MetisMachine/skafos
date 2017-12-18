#ifndef __CLI_ENV__
#define __CLI_ENV__

#include "common.h"
#include "file/file.h"
#include "request/request.h"
#include "git/git.h"
#include "auth/auth.h"
#include "templates/templates.h"

#define VERIFY_AUTH if(Env::instance()->authenticated() == false) Auth::authenticate
#define ENV_PATHS Env::instance()->paths

const std::string METIS_API_TOKEN     = "METIS_API_TOKEN";
const std::string METIS_AUTH_TOKEN    = "METIS_OAUTH";
const std::string METIS_CREDENTIALS   = "credentials.json";
const std::string METIS_TEMPLATE_REPO = "https://github.com/MetisMachine/templates.list.git";
const std::string METIS_TEMPLATE_DIR  = "templates";

struct metis_path {
  std::string home;
  std::string env;
  std::string templates;
  std::string credentials;
};

class Env {
public:
  bool authenticated();
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
