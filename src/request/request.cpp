#include "request.h"

using namespace std;
using namespace json11;
using namespace cpr;

Response Request::authenticate(string email, string password) {
  Url url = Url{(API_URL + "/users/login/")};
  
  Payload payload = Payload{
    {"email",       email}, 
    {"password",    password},
    {"client_id",   CLIENT_ID},
    {"grant_type",  "password"}
  };

  Header headers = Header{{"Content-Type", "application/x-www-form-urlencoded"}};

  return Post(url, payload, headers);
}
