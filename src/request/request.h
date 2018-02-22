#ifndef __CLI_REQUEST__
#define __CLI_REQUEST__

#include "common.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

class Request {
public:
  static RestClient::Response authenticate(std::string email, std::string password);
  static RestClient::HeaderFields api_headers();
  static RestClient::Response ping();
  static RestClient::Response tokens();
  static RestClient::Response generate_token();
  static RestClient::Response create_project(std::string name);
  static RestClient::Response env_vars(std::string project_id);
  static RestClient::Response env_var(std::string project_id, std::string key);
  static RestClient::Response add_env_var(std::string project_id, std::string key, std::string value);
  static RestClient::Response delete_env_var(std::string project_id, std::string key);
  static RestClient::Response fetch(std::string project_id, std::string table);
  static RestClient::Response create_task(std::string name, std::string project_id);
  static RestClient::Response kill_project(std::string project_token);
  static RestClient::Response kill_project(std::string porject_token, std::string project_tasks, std::string tasks);
  static RestClient::Response kill_project_task(std::string project_task);
  static RestClient::Response kill_project_task(std::string project_task, std::string tasks);
  static RestClient::Response kill_task(std::string task);
  static RestClient::Response kill_task(std::string task, std::string project_tasks);

  static void download(std::string url, std::string save_path);
  static std::vector<std::string> string_split(const std::string& s, char delimiter);

private:
  static Request *instance_;
  static Request *instance();
  
  RestClient::Connection *connection;
  Request();
  ~Request();

  RestClient::HeaderFields _default_headers();
  RestClient::HeaderFields _api_headers();
  RestClient::HeaderFields _oauth_headers();
  RestClient::Response _authenticate(std::string email, std::string password);
  RestClient::Response _ping();
  RestClient::Response _tokens();
  RestClient::Response _generate_token();
  RestClient::Response _create_project(std::string name);
  RestClient::Response _env_vars(std::string project_id);
  RestClient::Response _env_var(std::string project_id, std::string key);
  RestClient::Response _add_env_var(std::string project_id, std::string key, std::string value);
  RestClient::Response _delete_env_var(std::string project_id, std::string key);
  RestClient::Response _fetch(std::string project_id, std::string table);
  RestClient::Response _create_task(std::string name, std::string project_id);
  RestClient::Response _kill_project(std::string project_token);
  RestClient::Response _kill_project(std::string project_token, std::string project_tasks, std::string tasks);
  RestClient::Response _kill_project_task(std::string project_task);
  RestClient::Response _kill_project_task(std::string project_task, std::string tasks);
  RestClient::Response _kill_task(std::string task);
  RestClient::Response _kill_task(std::string task, std::string project_tasks);

  static void _download(std::string url, std::string save_path);
};

#endif
