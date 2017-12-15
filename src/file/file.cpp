#include <algorithm>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "file.h"

using namespace std;

int FileManager::create_path(mode_t mode, const string& rootPath, string& path) {
  struct stat st;

  for(std::string::iterator iter = path.begin(); iter != path.end();) {
    std::string::iterator newIter = std::find(iter, path.end(), '/');
    string newPath                = rootPath + "/" + std::string( path.begin(), newIter);

    if(stat(newPath.c_str(), &st) != 0) {
      if(mkdir( newPath.c_str(), mode) != 0 && errno != EEXIST) {
          std::cerr << "❗️ Error: cannot create folder [" << newPath << "] not a dir " << std::endl;
        return -1;
      }
    } else {
      if(!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        std::cerr << "❗️ Error: path [" << newPath << "] not a dir " << std::endl;
        return -1;
      }
    }
  
    iter = newIter;

    if(newIter != path.end()) {
      ++iter;
    }
  }

  return 0;
}

void FileManager::write(string path, string content) {
  ofstream fout(path);
  
  fout << content;
  
  fout.close();  
}

string FileManager::read(string path) {
  ifstream ifs(path);
  
  string content(
    (istreambuf_iterator<char>(ifs)),
    (istreambuf_iterator<char>())
  );

  return content;
}

bool FileManager::file_exists(std::string path) {
  return (access(path.c_str(), F_OK) != -1);
}

bool FileManager::dir_exists(std::string path) {
  struct stat info;
  
  if(stat(path.c_str(), &info) != 0) {
    return false;
  } else if(info.st_mode & S_IFDIR) {
    return true;
  }

  return false;
}

bool FileManager::is_dir(std::string path) {
  struct stat buffer;
  if(stat(path.c_str(), &buffer) != 0) {
    return true;
  }

  return S_ISDIR(buffer.st_mode);
}

bool FileManager::is_dot_dir(std::string path) {
  if(path == "." || path == "..") {
    return true;
  }

  return false;
}

void FileManager::delete_dir(std::string path) {

  size_t path_len;
  char  *full_path;
  DIR *dir;
  struct stat stat_path, stat_entry;
  struct dirent *entry;

  stat(path.c_str(), &stat_path);

  if(!is_dir(path)) {
    cerr << "Unable to delete directory: " << path << endl;
    
    exit(EXIT_FAILURE);
  }

  if((dir = opendir(path.c_str())) == 0) {
    cerr << "Unable to open directory: " << path << endl;
    
    exit(EXIT_FAILURE);
  }

  path_len = strlen(path.c_str());

  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }

    full_path = (char *)calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
    strcpy(full_path, path.c_str());
    strcat(full_path, "/");
    strcat(full_path, entry->d_name);

    stat(full_path, &stat_entry);

    if (S_ISDIR(stat_entry.st_mode) != 0) {
      delete_dir(string(full_path));
      continue;
    }

    unlink(full_path);
  }

  rmdir(path.c_str());
}


list<string> FileManager::dir_list(string path) {
  DIR *dir            = NULL;
  struct dirent *ent  = NULL;

  if(chdir(path.c_str()) < 0) {
    cerr << "Invalid directory: " << path << endl;

    exit(EXIT_FAILURE);
  }

  list<string> list;

  dir = opendir(".");
  if(dir) {
    while((ent = readdir(dir))) {
      if(!is_dot_dir(ent->d_name) ) {
        string filename(ent->d_name);
        string name = filename.substr(0, filename.find_last_of("."));
        string ext  = filename.substr(filename.find_last_of(".") + 1);

        if(ext == "yml") {
          list.push_back(name);
        }
      }
    }

    closedir(dir);
    return list;
  }

  cerr << "Unable to open directory: " << path << endl;
  exit(EXIT_FAILURE);
}
