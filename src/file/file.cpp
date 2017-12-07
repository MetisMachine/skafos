#include "file.h"

using namespace std;

int FileManager::create_path(mode_t mode, const string& rootPath, string& path) {
  struct stat st;

  for(string::iterator iter = path.begin() ; iter != path.end();) {
  string::iterator newIter  = find( iter, path.end(), '/' );
  string newPath            = rootPath + "/" + string( path.begin(), newIter);

    if( stat( newPath.c_str(), &st) != 0) {           
      if( mkdir( newPath.c_str(), mode) != 0 && errno != EEXIST ) {
        cout << "cannot create folder [" << newPath << "] : " << strerror(errno) << endl;
        return -1;
      }
    } else {
      if( !S_ISDIR(st.st_mode) ) {
        errno = ENOTDIR;
       
        cout << "path [" << newPath << "] not a dir " << endl;
       
        return -1;
      }
    }
  
    iter = newIter;

    if( newIter != path.end() ) {
      ++ iter;
    }
  }

  return 0;
}

void FileManager::write(string path, string content) {
  ofstream fout(path);
  
  fout << content;
  
  fout.close();  
}

bool FileManager::file_exists(std::string path) {
  return (access(path.c_str(), F_OK) != -1);
}
