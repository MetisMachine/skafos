#include <algorithm>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/resource.h>
#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>

#include "file.h"

using namespace std;

#define CHUNK 16384

string FileManager::cwd() {
  char buffer[FILENAME_MAX] = {0};
  getcwd(buffer, FILENAME_MAX);


  return string(buffer);
}

string FileManager::resolve_path(string path) {
  char resolved[PATH_MAX];

  realpath(path.c_str(), resolved);

  return string(resolved);
}

bool FileManager::create_path(mode_t mode, const string &root_path, string &path) {
  struct stat st;

  for(string::iterator iter = path.begin(); iter != path.end();) {

    string::iterator new_iter  = find(iter, path.end(), '/');
    string new_path            = root_path + "/" + string( path.begin(), new_iter);

    if(stat(new_path.c_str(), &st) != 0) {
      if(mkdir( new_path.c_str(), mode) != 0 && errno != EEXIST) {
        console::error("Unable to create folder: " + new_path + ". Exist and not a folder.");

        return false;
      }
    } else {
      if(!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;

        console::error("Unable to create folder: " + new_path + ". Exist and not a folder.");

        return false;
      }
    }
  
    iter = new_iter;

    if(new_iter != path.end()) {
      ++iter;
    }
  }

  return true;
}

bool FileManager::create_path(mode_t mode, string path) {
  int res = mkdir(path.c_str(), mode);

  if(res == -1) {
    switch(errno) {
      case ENOENT:
        if(create_path(0755, path.substr(0, path.find_last_of('/')))) {
          return 0 == mkdir(path.c_str(), mode);
        } else {
          return false;
        }
      case EEXIST:
        return true;
      default:
        return false;
    }
  }

  return true;
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

bool FileManager::file_exists(string path) {
  return (access(path.c_str(), F_OK) != -1);
}

bool FileManager::dir_exists(string path) {
  struct stat info;
  
  if(stat(path.c_str(), &info) != 0) {
    return false;
  } else if(info.st_mode & S_IFDIR) {
    return true;
  }

  return false;
}

bool FileManager::is_dir(string path) {
  struct stat buffer;
  if(stat(path.c_str(), &buffer) == 0) {
    return S_ISDIR(buffer.st_mode);
  } 
  
  return false;
}

bool FileManager::is_dot_dir(string path) {
  if(path == "." || path == "..") {
    return true;
  }

  return false;
}

void FileManager::delete_dir(string path) {

  size_t path_len;
  char  *full_path;
  DIR *dir;
  struct stat stat_path, stat_entry;
  struct dirent *entry;

  stat(path.c_str(), &stat_path);

  if(!is_dir(path)) {
    console::error("Unable to delete directory: " + path);

    exit(EXIT_FAILURE);
  }

  if((dir = opendir(path.c_str())) == 0) {
    console::error("Unable to open directory: " + path);
    
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

void FileManager::delete_file(std::string path) {
  if(is_dir(path)) {
    return;
  }
  
  unlink(path.c_str());
}

list<string> FileManager::dir_list(string path, string extension) {
  DIR *dir            = NULL;
  struct dirent *ent  = NULL;

  if(chdir(path.c_str()) < 0) {
    console::error("Invalid directory: " + path);

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

        if(extension == "" || ext == extension) {
          list.push_back(name);
        }
      }
    }

    closedir(dir);
    return list;
  }

  console::error("Unable to open directory: " + path);

  exit(EXIT_FAILURE);
}

bool FileManager::unzip(string source, string destination) {
  const char *filename  = source.c_str();
  const char *dest      = destination.c_str();

  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int flags;
  int r;
  string root;

  /* Select which attributes we want to restore. */
  flags = ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;
  flags |= ARCHIVE_EXTRACT_ACL;
  flags |= ARCHIVE_EXTRACT_FFLAGS;

  a = archive_read_new();
  archive_read_support_format_all(a);
  archive_read_support_compression_all(a);

  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);
  
  if ((r = archive_read_open_filename(a, filename, 10240))) {
    console::error("Unable to read archive: " + string(filename));

    return false;
  }

  while(true) {
    r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF) {
        break;
    } else if (r < ARCHIVE_OK) {
      console::error(
        "Failed to extract from archive ("
        + source
        + "); "
        + archive_error_string(a)
      );
      
      return false; 
    } else if (r < ARCHIVE_WARN) {
      console::error(
        "Failed to extract from archive ("
        + source
        + "); "
        + archive_error_string(a)
      );

      return false;
    }

    const char* current_file = archive_entry_pathname(entry);
    if(root.empty()) {
      root = current_file;
    }

    string full_output = destination + (current_file + (root.empty() ? 0 : root.size() - 1));
    archive_entry_set_pathname(entry, full_output.c_str());

    r = archive_write_header(ext, entry);
    if (r < ARCHIVE_OK) {
      console::error(
        "Failed to extract from archive ("
        + source
        + "); "
        + archive_error_string(a)
      );

      return false;
    } else if (archive_entry_size(entry) > 0) {
      r = copy_data(a, ext);
      
      if (r < ARCHIVE_OK) {
        console::error(
          "Failed to extract from archive ("
          + source
          + "); "
          + archive_error_string(a)
        );

        return false;
      } else if (r < ARCHIVE_WARN) {
        console::error(
          "Failed to extract from archive ("
          + source
          + "); "
          + archive_error_string(a)
        );

        return false;
      }
    }

    r = archive_write_finish_entry(ext);
    if (r < ARCHIVE_OK) {
      console::error(
        "Failed to extract from archive ("
        + source
        + "); "
        + archive_error_string(a)
      );

      return false;
    } else if (r < ARCHIVE_WARN) {
      console::error(
        "Failed to extract from archive ("
        + source
        + "); "
        + archive_error_string(a)
      );

      return false;
    }
  }

  archive_read_close(a);
  archive_read_free(a);
  archive_write_close(ext);
  archive_write_free(ext);

  return 0;
}

int FileManager::copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  int64_t offset;

  while(true) {
    r = archive_read_data_block(ar, &buff, &size, &offset);

    if (r == ARCHIVE_EOF) {
      return (ARCHIVE_OK);
    }

    if (r < ARCHIVE_OK) {
      return (r);
    }

    r = archive_write_data_block(aw, buff, size, offset);

    if (r < ARCHIVE_OK) {
      cerr << archive_error_string(aw) << endl;

      return (r);
    }
  }
}

