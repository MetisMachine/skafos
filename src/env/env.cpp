#include "env.h"

using namespace std;
using namespace json11;

Env *Env::instance_ = 0;

Env *Env::instance() {
  if (instance_ == 0) {
    instance_ = new Env();
  }

  return instance_;
}

Env::Env() {
  string home   = this->home_dir();
  string metis  = this->metis_dir();

  paths.home        = home;
  paths.env         = metis;
  paths.templates   = home + "/" + metis + "/" + METIS_TEMPLATE_DIR;
  paths.credentials = home + "/" + metis + "/" + METIS_CREDENTIALS;
  paths.defaults    = home + "/" + metis + "/" + METIS_DEFAULTS;
  paths.cache       = home + "/" + metis + "/" + METIS_TEMPLATE_DIR + "/" + METIS_CACHE_DIR;
}

void Env::setup() {
  static bool running = false;

  if(!running) {
    running = true;
    console::info("Setting up environment");

    Template::update();
    Template::create_cache_dir();
  }
}

bool Env::authenticated() {
  return (FileManager::file_exists(paths.credentials) && (Request::ping().body == "pong"));
}

string Env::get(string key) {
  char *res = getenv(key.c_str());

  return (res == NULL)? string() : string(res);
}

void Env::set(string key, string val) {
  setenv(key.c_str(), val.c_str(), true);
}

bool Env::load_credentials() {
  if(FileManager::file_exists(paths.credentials)) {
    string creds = FileManager::read(paths.credentials);

    string err;
    Json json = Json::parse(creds, err);

    if(err.length() > 0) {
      console::error("Unable to load or read credentials :" + err);
      
      return false;
    }

    string token = json["token"].string_value();
    
    if(token.length() < 1) {
      console::error("Credentials error: no token found.");
      
      return false;
    }

    this->set(METIS_AUTH_TOKEN, token);
    this->set(METIS_API_TOKEN, token);
    
    return Request::ping().body == "pong";
  }

  return false;
}

void Env::write_credentials(Json object) {  
  FileManager::create_path(0755, paths.home, paths.env);

  Json creds = Json::object{
    {"token", object["token"]}
  };

  string creds_string = creds.dump();
  FileManager::write(paths.credentials, creds_string);
}

void Env::write_default_org(std::string org_name) {
  FileManager::create_path(0755, paths.home, paths.env);

  Json org = Json::object{
    {"org_name", org_name}
  };

  auto str = org.dump();
  FileManager::write(paths.defaults, str);
}

void Env::verify_auth() {
  if(Env::instance()->authenticated() == false) {
    Auth::authenticate(); 
    Env::setup();
  }
}

string Env::home_dir() {
  struct passwd *pw = getpwuid(getuid());

  return string(pw->pw_dir);
}

string Env::metis_dir() {
  return (ENVIRONMENT == "production")? ".metis" : ".metis_dev";
}
