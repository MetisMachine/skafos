#ifndef __CLI_REPO__
#define __CLI_REPO__

#include "common.h"
#include <git2.h>
#include <git2/clone.h>

#ifndef PRIuZ
/* Define the printf format specifer to use for size_t output */
#if defined(_MSC_VER) || defined(__MINGW32__)
#	define PRIuZ "Iu"
#else
#	define PRIuZ "zu"
#endif
#endif

namespace Git {
  typedef struct progress_data {
    git_transfer_progress fetch_progress;
    size_t completed_steps;
    size_t total_steps;
    const char *path;
  } progress_data;

  class Repo {
  private:
    static void print_progress(const progress_data *pd);
    static int sideband_progress(const char *str, int len, void *payload);
    static int fetch_progress(const git_transfer_progress *stats, void *payload);
    static void checkout_progress(const char *path, size_t cur, size_t tot, void *payload);
  public:
    static int clone(std::string url,  std::string path);
  };
  
}

#endif
