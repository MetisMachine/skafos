#include <curl/curl.h>

#include "request.h"
#include "env/env.h"
#include "project_env/project_env.h"

using namespace std;
using namespace json11;

#define ENDPOINT(str) Url{(API_URL + str)}

const string LOGIN_URL             = "/users/login";
const string PING_URL              = "/ping";
const string TOKEN_URL             = "/api_tokens/";
const string PROJECT_URL           = "/projects";
const string ENV_VARS_URL          = "/env_vars";
const string FETCH_URL             = "/data";
const string JOBS_URL              = "/jobs";
const string KILL_ALL_URL          = "/kill_all";
const string KILL_DEPLOYMENT_URL   = "/kill";
const string DEPLOYMENTS_URL       = "/deployments";
const string OLD_ORGANIZATIONS_URL = "/old/organizations";
const string ORGANIZATIONS_URL     = "/organizations";

#define DEFAULT_HEADERS() \
RestClient::HeaderFields headers = this->_default_headers(); \
this->connection->SetHeaders(headers)

#define API_HEADERS() \
RestClient::HeaderFields headers = this->_api_headers(); \
this->connection->SetHeaders(headers)

#define OAUTH_HEADERS() \
RestClient::HeaderFields headers = this->_oauth_headers(); \
this->connection->SetHeaders(headers)

// Private
Request *Request::instance_ = 0;

Request *Request::instance() {
  if (instance_ == 0) {
    instance_ = new Request();
  }

  return instance_;
}

Request::Request() {
  RestClient::init();

  this->connection = new RestClient::Connection(API_URL);
  this->connection->SetTimeout(30);
  this->connection->SetUserAgent("Skafos");
  this->connection->FollowRedirects(true);
}

Request::~Request() {
  RestClient::disable();
}

RestClient::HeaderFields Request::_default_headers() {
  RestClient::HeaderFields headers;
  headers["Content-Type"] = "application/json";

  return headers;
}

RestClient::HeaderFields Request::_api_headers() {
  RestClient::HeaderFields headers  = this->_default_headers();
  headers["x-api-token"]            = Env::instance()->get(METIS_API_TOKEN);

  return headers;
}

RestClient::HeaderFields Request::_oauth_headers() {
  RestClient::HeaderFields headers  = this->_default_headers();
  headers["Authorization"]          = "Bearer " + Env::instance()->get(METIS_AUTH_TOKEN);

  return headers;
}

RestClient::Response Request::_authenticate(string email, string password) {
  DEFAULT_HEADERS();

  Json body = Json::object{
    {"email",       email},
    {"password",    password},
    {"client_id",   CLIENT_ID},
    {"grant_type",  "password"}
  };

  return this->connection->post(LOGIN_URL, body.dump());
}

RestClient::Response Request::_ping() {
  API_HEADERS();

  return this->connection->get(PING_URL);
}

RestClient::Response Request::_tokens() {
  API_HEADERS();

  return this->connection->get(TOKEN_URL);
}

RestClient::Response Request::_generate_token() {
  OAUTH_HEADERS();

  return this->connection->post(TOKEN_URL, "");
}

RestClient::Response Request::_create_project(string name) {
  API_HEADERS();

  Json body = Json::object{
    {"name",  name}
  };

  return this->connection->post(PROJECT_URL, body.dump());
}

RestClient::Response Request::_create_job(string name, string project_token="") {
  API_HEADERS();

  if (project_token.size() < 1){
    project_token = PROJECT_TOKEN;
  }
  Json body = Json::object{
    {"name",  name},
    {"project_token", project_token}
  };

  return this->connection->post(JOBS_URL, body.dump());
}

RestClient::Response Request::_env_vars(string project_id) {
  API_HEADERS();

  string uri = ENV_VARS_URL + "/" + project_id;

  return this->connection->get(uri);
}

RestClient::Response Request::_env_var(std::string project_id, std::string key) {
  API_HEADERS();

  string uri = ENV_VARS_URL + "/" + project_id + "/" + key;

  return this->connection->get(uri);
}

RestClient::Response Request::_add_env_var(string project_id, string key, string value) {
  API_HEADERS();

  Json body = Json::object{
    {"name",  key},
    {"value", value}
  };

  string uri(ENV_VARS_URL + "/" + project_id);

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_delete_env_var(string project_id, string key) {
  API_HEADERS();

  string uri = ENV_VARS_URL + "/" + project_id + "/" + key;

  return this->connection->del(uri);
}

RestClient::Response Request::_fetch(string project_id, string table) {
  API_HEADERS();

  string uri = FETCH_URL + "/" + project_id + "/" + table;

  return this->connection->get(uri);
}

RestClient::Response Request::_create_deployment(string name, string project_id) {
  API_HEADERS();

  Json body = Json::object {
    {"name", name}
  };

  string uri = PROJECT_URL + "/" + project_id + "/" + JOBS_URL + "/";

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_project(string project_token){
  API_HEADERS();
  string uri = "";
  Json body = Json::object{
    {"project_token", project_token}
  };

  uri = PROJECT_URL + "/" + project_token + KILL_DEPLOYMENT_URL;

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_project(string project_token, string jobs, string deployments){
  API_HEADERS();
  string uri = "";
  Json body;
  Json deployment;
  vector<string> deployments_list;
  vector<string> jobs_list;
  vector<Json> deployments_with_jobs;
  vector<Json> deployments_without_jobs;

  uri = PROJECT_URL + "/" + project_token + KILL_DEPLOYMENT_URL;

  if(jobs.compare("") == 0 && deployments.compare("") != 0){
    deployments_list = string_split(deployments, ',');
    for(int i = 0; i < deployments_list.size(); i++){
      deployment = Json::object{
        {"deployment_id", deployments_list[i]}
      };
      deployments_without_jobs.push_back(deployment);
    }
    body = Json::object{
      {"project_token", project_token},
      {"deployments", deployments_without_jobs}
    };
  } else if (jobs.compare("") != 0 && deployments.compare("") == 0){
    jobs_list = string_split(jobs, ',');
    body = Json::object{
      {"project_token", project_token},
      {"job_ids", jobs_list}
    };
  } else {
    deployments_list = string_split(deployments, ',');
    jobs_list = string_split(jobs, ',');
    for(int i = 0; i < deployments_list.size(); i++){
      deployment = Json::object{
        {"deployment_id", deployments_list[i]},
        {"job_ids", jobs_list}
      };
      deployments_with_jobs.push_back(deployment);
    }
    body = Json::object{
      {"project_token", project_token},
      {"deployments", deployments_with_jobs}
    };
  }

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_job(string job){
  API_HEADERS();
  string uri = "";
  Json body = Json::object{
    {"job_id", job}
  };

  uri = JOBS_URL + "/" + job + KILL_DEPLOYMENT_URL;

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_job(string job, string deployments){
  API_HEADERS();
  string uri = "";
  vector<string> deployments_list;

  uri = JOBS_URL + "/" + job + KILL_DEPLOYMENT_URL;

  deployments_list = string_split(deployments, ',');

  Json body = Json::object {
    {"job_id", job},
    {"deployment_ids", deployments_list}
  };

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_deployment(string deployment){
  API_HEADERS();
  string uri = "";
  Json body = Json::object{
    {"uuid", deployment}
  };

  uri = DEPLOYMENTS_URL + "/" + deployment + KILL_DEPLOYMENT_URL;

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_kill_deployment(string deployment, string jobs){
  API_HEADERS();
  string uri = "";
  vector<string> jobs_list;

  uri = DEPLOYMENTS_URL + "/" + deployment + KILL_DEPLOYMENT_URL;

  jobs_list = string_split(jobs, ',');

  Json body = Json::object {
    {"uuid", deployment},
    {"job_ids", jobs_list}
  };

  return this->connection->post(uri, body.dump());
}

RestClient::Response Request::_organization_info(){
  API_HEADERS();

  return this->connection->get(OLD_ORGANIZATIONS_URL);
}

RestClient::Response Request::_my_organizations() {
  API_HEADERS();

  return this->connection->get(ORGANIZATIONS_URL);
}

// DOWNLOAD
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

void Request::_download(string repo_url, string save_path) {
  FILE *fp;
  CURLcode res;
  char output[FILENAME_MAX];
  char *url   = (char *)repo_url.c_str();
  CURL *curl  = curl_easy_init();

  console::debug("Request _download(save_path): " + save_path);
  strcpy(output, FileManager::resolve_path(save_path).c_str());

  if(curl) {
    fp = fopen(output, "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
      console::error(
        "Unable to download "
        + repo_url
        + " ("
        + curl_easy_strerror(res)
        + ").");
    }

    fclose(fp);
    curl_easy_cleanup(curl);
  }
}

// Public
RestClient::Response Request::authenticate(string email, string password) {
  return instance()->_authenticate(email, password);
}

RestClient::Response Request::ping() {
  return instance()->_ping();
}

RestClient::Response Request::tokens() {
  return instance()->_tokens();
}


RestClient::Response Request::generate_token() {
  return instance()->_generate_token();
}

RestClient::Response Request::create_project(string name) {
  return instance()->_create_project(name);
}

RestClient::Response Request::create_job(string name, string project_token) {
  return instance()->_create_job(name, project_token);
}

RestClient::Response Request::env_vars(string project_id) {
  return instance()->_env_vars(project_id);
}

RestClient::Response Request::env_var(string project_id, string key) {
  return instance()->_env_var(project_id, key);
}

RestClient::Response Request::add_env_var(string project_id, string key, string value) {
  return instance()->_add_env_var(project_id, key, value);
}

RestClient::Response Request::delete_env_var(string project_id, string key) {
  return instance()->_delete_env_var(project_id, key);
}

RestClient::Response Request::fetch(string project_id, string table) {
  return instance()->_fetch(project_id, table);
}

RestClient::Response Request::create_deployment(string name, string project_id) {
  return instance()->_create_deployment(name, project_id);
}

RestClient::Response Request::kill_project(string project_token) {
  return instance()->_kill_project(project_token);
}

RestClient::Response Request::kill_project(string project_token, string jobs, string deployments) {
  return instance()->_kill_project(project_token, jobs, deployments);
}

RestClient::Response Request::kill_job(string job){
  return instance()->_kill_job(job);
}

RestClient::Response Request::kill_job(string job, string deployments){
  return instance()->_kill_job(job, deployments);
}

RestClient::Response Request::kill_deployment(string deployment){
  return instance()->_kill_deployment(deployment);
}

RestClient::Response Request::kill_deployment(string deployment, string jobs){
  return instance()->_kill_deployment(deployment, jobs);
}

RestClient::Response Request::organization_info(){
  return instance()->_organization_info();
}

RestClient::Response Request::my_organizations() {
  return instance()->_my_organizations();
}

// DOWNLOAD
void Request::download(string url, string save_path) {
  instance()->_download(url, save_path);
}

std::vector<std::string> Request::string_split(const std::string& s, char delimiter){
   std::vector<std::string> split_list;
   std::string word;
   std::istringstream list_stream(s);
   while (std::getline(list_stream, word, delimiter))
   {
      split_list.push_back(word);
   }
   return split_list;
}
