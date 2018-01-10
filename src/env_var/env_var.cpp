#include <strings.h>
#include "env_var.h"
#include "request/request.h"
#include "project_env/project_env.h"

using namespace json11;
using namespace std;

void EnvVar::run(int argc, char **argv, int cmd_index) {
  list();
}

void EnvVar::list() {
  RestClient::Response response = Request::env_vars(PROJECT_TOKEN);

  string err;
  Json json = Json::parse(response.body, err);
  auto list = json["data"].array_items();

  if(list.empty()) {
    console::info("No env vars currently set");

    exit(EXIT_SUCCESS);
  }

  console::headline("Current env vars");

  for(auto i : list) {
    string key = i["name"].string_value();
    string val = i["value"].string_value();
    
    console::info(key + "=" + val);  
  }

  cout << endl;
}

void EnvVar::get(std::string key) {
  RestClient::Response response = Request::env_var(PROJECT_TOKEN,  key);

  string err;
  Json json = Json::parse(response.body, err);
  auto data = json["data"];

  if(data["name"].string_value() == "") {
    console::info("Could not find environment variable: " + key);
  } else {
    console::info(data["name"].string_value() + "=" + data["value"].string_value());
  }
}

void EnvVar::set(std::string key, std::string value) {
  Request::add_env_var(PROJECT_TOKEN, key, value);

  console::info("Set " + key + " to value: " + value);
}

void EnvVar::del(std::string key) {
  string yesno = "y";

  cout << "Are you sure you want to delete: " << key << "? [Yn] ";

  string input;
  getline(cin, input);

  if(!input.empty()) {
    istringstream stream(input);
    
    stream >> yesno;
  }

  if(strcasecmp(yesno.c_str(), "y") == 0) {
    Request::delete_env_var(PROJECT_TOKEN, key);

    console::success("Env var (" + key + ") has been deleted");
  }

  exit(EXIT_SUCCESS);
}
