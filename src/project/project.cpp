#include "project.h"
#include "templates/templates.h"
#include "file/file.h"
#include "env/env.h"

using namespace std;

void Project::init(string name, string tpl) {
  string directory = (name == "")? 
    FileManager::cwd() : (name == ".") ? 
      FileManager::resolve_path(name) : 
      FileManager::cwd() + "/" + name;


  string project_name = directory.substr(directory.find_last_of("/"));
    
  if(!FileManager::dir_exists(directory)) {
    FileManager::create_path(0755, directory);
  }

  TemplateDetails tpl_details = Template::find(tpl);
  if(tpl_details.repo.length() < 1) {
    cerr << "Unable to find template: " << tpl << endl;

    exit(EXIT_FAILURE);
  }

  string version = (tpl_details.version.length() > 0)? tpl_details.version : "master";

  Template::download(tpl_details, version);
  
  string cache_path = ENV_PATHS.cache + "/" + tpl + ".zip";
 
  FileManager::unzip(cache_path, directory);
}


/*
  cout << "CWD: " << FileManager::cwd() << endl;
  cout << "Res: " << FileManager::resolve_path(".") << endl;
  // TemplateDetails tpl = Template::find("base");

  // Template::download(tpl, "0.0.1");
  
  // string zip_path = ENV_PATHS.cache + "/base.zip";
  // string dest     = ENV_PATHS.cache;
  
  // FileManager::unzip(zip_path, dest);

*/
