#include "templates.h"
#include "yaml-cpp/yaml.h"
#include "env/env.h"
#include "file/file.h"
#include <iomanip>

using namespace std;

const string TEMPLATE_HEAD = "head";

void Template::update() {
  console::info("Updating templates...");
  string tpl_path = ENV_PATHS.home + "/" + ENV_PATHS.env + "/template_list.zip"; 
  
  Request::download(METIS_TEMPLATE_LIST, tpl_path);
  FileManager::unzip(tpl_path, ENV_PATHS.templates);
  FileManager::delete_file(tpl_path);
}

void Template::search(string name) {
  list<string> tpls = FileManager::dir_list(ENV_PATHS.templates, "yml");

  tpls.remove_if([&name](string str) {
    return str.find(name) == string::npos;
  });

  if(tpls.size() < 1) {
    console::error("No template found with name: " + name);
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

TemplateDetails Template::find(string name) {
  TemplateDetails details;

  for(TemplateDetails tpl : all()) {
    if(tpl.name == name) {
      return tpl;
    }
  }

  return details;
}

list<TemplateDetails> Template::all() {
  list<string> tpls = FileManager::dir_list(ENV_PATHS.templates, "yml");

  if(tpls.size() < 1) {
    update();
    return all();
  }


  list<TemplateDetails> tpl_list;

  for(string t : tpls) {
    string path(ENV_PATHS.templates + "/" + t + ".yml");

    auto details = parse_template(path);

    tpl_list.push_back(details);
  }

  return tpl_list;
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

void Template::download(TemplateDetails details, string version) {
  console::info("Downloading template...");
  
  create_cache_dir();

  string cache_path   = ENV_PATHS.cache;
  string tpl_path     = cache_path + "/" + details.name + ".zip"; 

  if(!FileManager::file_exists(tpl_path)) {
    string download_url = details.repo;
    if((download_url.substr((download_url.length() - 1), download_url.length())) == "/") {
      download_url = download_url.substr(0, download_url.length() - 1);
    }
  
    download_url += "/archive/" + version + ".zip";

    Request::download(download_url, tpl_path);

    download(details, version);
  }
}

void Template::create_cache_dir() {
  if(FileManager::dir_exists(ENV_PATHS.cache)) {
    return;
  }

  string cache = METIS_CACHE_DIR;
  FileManager::create_path(0755, ENV_PATHS.templates, cache);
}



// Search later? https://api.github.com/search/code\?q\=base+in:file+repo:metismachine/templates.list
