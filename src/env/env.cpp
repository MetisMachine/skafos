#include "env.h"
#include "file/file.h"
#include "request/request.h"

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
  string path   = home + "/" + metis + "/" + METIS_CREDENTIALS;

  paths.home         = home;
  paths.env          = metis;
  paths.credentials  = path;
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
      cerr << "❗️ Credentials error: " << err << endl;
      
      return false;
    }

    string token = json["token"].string_value();
    
    if(token.length() < 1) {
      cerr << "❗️ Credentials error: no token found." << endl;
      
      return false;
    }

    this->set(METIS_AUTH_TOKEN, token);

    return Request::ping().body == "pong";
  }

  return false;
}

void Env::write_credentials(Json object) {

  FileManager::create_path(0600, paths.home, paths.env);

  Json creds = Json::object{
    {"token", object["token"]}
  };

  string creds_string = creds.dump();
  FileManager::write(paths.credentials, creds_string);
}


string Env::home_dir() {
  struct passwd *pw   = getpwuid(getuid());

  return string(pw->pw_dir);
}

string Env::metis_dir() {
  return ".metis";
}
