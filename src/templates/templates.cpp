#include "templates.h"
#include "yaml-cpp/yaml.h"
#include "env/env.h"
#include "git/git.h"
#include "file/file.h"
#include <iomanip>

using namespace std;

void Template::update() {
  VERIFY_AUTH();

  FileManager::dir_exists(ENV_PATHS.templates) ? pull() : clone();

  if(FileManager::dir_exists(ENV_PATHS.templates)) {
    cout << "Updating project templates" << endl;
    pull();    
  } else {
    cout << "Fetching project templates" << endl;
    clone();
  }
}

void Template::search(string name) {
  list<string> tpls = FileManager::dir_list(ENV_PATHS.templates);
  tpls.remove_if([&name](string str) {
    return str.find(name) == string::npos;
  });

  if(tpls.size() < 1) {
    cout << "No templates found with name " << name << endl;
    return;
  }

  cout << endl;
  for(string t : tpls) {
    string path(ENV_PATHS.templates + "/" + t + ".yml");

    auto details = parse_template(path);

    cout << left
    << termcolor::green 
    << "➜ "
    << details.name 
    << " ("
    <<  details.version
    << ") "
    << termcolor::reset 
    << endl
    << "  " << details.description 
    << endl
    << "  url: " << details.repo
    << endl
    << endl;
  }
}

int Template::clone() {
  return Git::Repo::clone(METIS_TEMPLATE_REPO, ENV_PATHS.templates);
}

int Template::pull() {
  FileManager::delete_dir(ENV_PATHS.templates);

  return clone();
}

TemplateDetails Template::parse_template(std::string path) {
  TemplateDetails details;

  YAML::Node tpl = YAML::LoadFile(path);

  const string name         = tpl["name"].as<string>();
  const string author       = tpl["author"].as<string>();
  const string description  = tpl["description"].as<string>();
  const string tags         = tpl["tags"].as<string>();
  const string repo         = tpl["repo"].as<string>();
  const string version      = tpl["version"].as<string>();
  
  details.name        = name;
  details.author      = author;
  details.description = description;
  details.tags        = tags;
  details.repo        = repo;
  details.version     = version;

  return details;
}
