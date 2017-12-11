#include "request.h"
#include "env/env.h"

using namespace std;
using namespace json11;
using namespace cpr;

#define ENDPOINT(str) Url{(API_URL + str)}
#define DEFAULT_HEADERS Header{{"Content-Type", "application/json"}}

#define LOGIN_URL "/users/login"
#define PING_URL  "/ping"
#define TOKEN_URL "/api_tokens/"

Response Request::authenticate(string email, string password) {
  Url url         = ENDPOINT(LOGIN_URL);
  Header headers  = Header{{"Content-Type", "application/x-www-form-urlencoded"}};
  
  Payload payload = Payload{
    {"email",       email}, 
    {"password",    password},
    {"client_id",   CLIENT_ID},
    {"grant_type",  "password"}
  };

  return Post(url, payload, headers);
}

Response Request::ping() {
  Url url         = ENDPOINT(PING_URL);
  Header headers  = DEFAULT_HEADERS;

  add_oauth_token(&headers);

  return Get(url, headers);
}

//MARK - Tokens
Response Request::tokens() {
  Url url         = ENDPOINT(TOKEN_URL);
  Header headers  = DEFAULT_HEADERS;

  add_oauth_token(&headers);

  return Get(url, headers);
}

Response Request::generate_token() {
  Url url         = ENDPOINT(TOKEN_URL);
  Header headers  = DEFAULT_HEADERS;

  add_oauth_token(&headers);

  return Post(url, Payload{}, headers);
}


//MARK - Private
void Request::add_oauth_token(cpr::Header* headers) {
  string token = Env::instance()->get(METIS_AUTH_TOKEN);

  headers->insert({"authorization", ("Bearer " + token)});
}

void Request::add_api_token(cpr::Header* headers) {
  string token = Env::instance()->get(METIS_API_TOKEN);

  headers->insert({"x-api-token", token});
}
