#include "auth.h"
#include "file/file.h"
#include "request/request.h"
#include "env/env.h"

using namespace json11;
using namespace std;

void Auth::authenticate() {
  string email;
  string password;

  cout << "Please enter email: ";
  cin >> email;

  password = Auth::password_input("Please enter password: ");

  auto oauth = Request::authenticate(email, password);

  if (oauth.code >= 400) {
    cerr << "Error [" << oauth.code << "] making request" << endl;
  } else {
    string err;

    cout << "Writing credentials..." << endl;
  
    Env::instance()->write_credentials(Json::parse(oauth.body, err));
    Env::instance()->load_credentials();

    auto api_token = Request::generate_token();

    Env::instance()->write_credentials(Json::parse(api_token.body, err));

    cout << "Loading credentials..." << endl;    
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
