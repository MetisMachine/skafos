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


}

void EnvVar::get(std::string key) {
}

void EnvVar::set(std::string key, std::string value) {
  RestClient::Response response = Request::add_env_var(PROJECT_TOKEN, key, value);

  console::debug("ENV_VAR SET: " + response.body);
}

void EnvVar::del(std::string key) {
  
}
