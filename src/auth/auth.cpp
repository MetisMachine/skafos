#include "auth.h"
#include "file/file.h"
#include "request/request.h"
#include "env/env.h"

using namespace json11;
using namespace std;

void Auth::authenticate() {
  console::info("Skafos Authentication...");
  
  string email;
  string password;

  cout << console::ARROW << " Please enter email: ";
  cin >> email;

  password = Auth::password_input(console::ARROW + " Please enter password: ");

  cout << endl;

  console::info("Authenticating...");

  auto oauth = Request::authenticate(email, password);

  if (oauth.code >= 400) {
    console::error("Invalid email or password.");
    console::debug("Error making request. (" + to_string(oauth.code) + ")");
    exit(EXIT_FAILURE);
  } else {
    string err;

    Env::instance()->write_credentials(Json::parse(oauth.body, err));
    Env::instance()->load_credentials();

    auto api_token = Request::generate_token();

    console::info("Generating API token...");

    string token = api_token.body;
    
    console::info("Saving credentials...");

    Env::instance()->write_credentials(Json::parse(token, err));

    console::debug("Loading credentials...");
    Env::instance()->load_credentials();
  }
}

bool Auth::verify_auth() {
  return true;
}

bool Auth::verify_api_token() {
  return true;
}

string Auth::password_input(string prompt) {
  char *rawPassword = getpass(prompt.c_str());
  string str(rawPassword);

  return str;
}
