#include "templates.h"
#include "env/env.h"
#include <vector>

#include "git/git.h"
#include "file/file.h"

using namespace std;

void Template::update() {
  VERIFY_AUTH();

  FileManager::dir_exists(ENV_PATHS.templates) ? pull() : clone();
}

void Template::search(string name) {
  string path = ENV_PATHS.templates + "/" + "*" + name + "*";

  cout << "Searching for " << path << endl;
}

int Template::clone() {
  cout << "Fetching project templates" << endl;
  
  return Git::Repo::clone(METIS_TEMPLATE_REPO, ENV_PATHS.templates);
}

int Template::pull() {
  cout << "Updating project templates" << endl;

  return Git::Repo::pull(ENV_PATHS.templates);
}
