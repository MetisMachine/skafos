#pragma once

#include "common.h"

class Request {
public:
  static cpr::Response authenticate(std::string email, std::string password);
  static cpr::Response ping();
  static cpr::Response tokens();
  static cpr::Response generate_token();
private:
  static void add_oauth_token(cpr::Header* headers);
  static void add_api_token(cpr::Header* headers);
};
