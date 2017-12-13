#pragma once

#include "common.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

class Request {
public:
  static RestClient::Response authenticate(std::string email, std::string password);
  static RestClient::Response ping();
  static RestClient::Response tokens();
  static RestClient::Response generate_token();

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
};
