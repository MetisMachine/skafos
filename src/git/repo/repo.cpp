

#include "repo.h"

using namespace std;

namespace Git {
  void Repo::print_progress(const progress_data *pd) {
    int network_percent = pd->fetch_progress.total_objects > 0 ?
      (100*pd->fetch_progress.received_objects) / pd->fetch_progress.total_objects : 0;
    int index_percent = pd->fetch_progress.total_objects > 0 ?
      (100*pd->fetch_progress.indexed_objects) / pd->fetch_progress.total_objects : 0;
  
    int checkout_percent = pd->total_steps > 0
      ? (100 * pd->completed_steps) / pd->total_steps : 0;

    int kbytes = pd->fetch_progress.received_bytes / 1024;
  
    if (pd->fetch_progress.total_objects && pd->fetch_progress.received_objects == pd->fetch_progress.total_objects) {
      printf(
        "Resolving deltas %d/%d\r", 
        pd->fetch_progress.indexed_deltas,
        pd->fetch_progress.total_deltas
      );
    } 
  }
  
  int Repo::sideband_progress(const char *str, int len, void *payload) {
    (void)payload; // unused, callback wants it anyways.
  
    printf("remote: %.*s", len, str);
    fflush(stdout);
    return 0;
  }
  
  int Repo::fetch_progress(const git_transfer_progress *stats, void *payload) {
    progress_data *pd   = (progress_data*)payload;
    pd->fetch_progress  = *stats;

    print_progress(pd);
    return 0;
  }

  void Repo::checkout_progress(const char *path, size_t cur, size_t tot, void *payload) {
    progress_data *pd   = (progress_data*)payload;
    pd->completed_steps = cur;
    pd->total_steps     = tot;
    pd->path            = path;

    print_progress(pd);
  }
  
  
  int Repo::clone(string url, string path) {
    git_libgit2_init();

    progress_data pd                    = {{0}};
    git_repository *cloned_repo         = NULL;
    git_clone_options clone_opts        = GIT_CLONE_OPTIONS_INIT;
    git_checkout_options checkout_opts  = GIT_CHECKOUT_OPTIONS_INIT;
  
    checkout_opts.checkout_strategy                   = GIT_CHECKOUT_SAFE;
    checkout_opts.progress_cb                         = checkout_progress;
    checkout_opts.progress_payload                    = &pd;
    clone_opts.checkout_opts                          = checkout_opts;
    clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
    clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;
    clone_opts.fetch_opts.callbacks.payload           = &pd;

    const char *c_url   = url.c_str();
    const char *c_path  = path.c_str();
    int error           = git_clone(&cloned_repo, c_url, c_path, &clone_opts);

    printf("\n");
    if (error != 0) {
      const git_error *err = giterr_last();
      
      if (err) {
        printf("ERROR %d: %s\n", err->klass, err->message);
      } else {
        printf("ERROR %d: no detailed info\n", error);
      }
    } else if (cloned_repo) {
      git_repository_free(cloned_repo);
    }

    git_libgit2_shutdown();
    return error;
  }

  int Repo::pull(string path) {
    git_libgit2_init();
  
    progress_data pd                    = {{0}};
    git_repository *repo                = NULL;
    git_checkout_options checkout_opts  = GIT_CHECKOUT_OPTIONS_INIT;
    checkout_opts.checkout_strategy     = GIT_CHECKOUT_FORCE;
    checkout_opts.progress_cb           = checkout_progress;
    checkout_opts.progress_payload      = &pd;
  
    git_repository_open(&repo, path.c_str());
  
    int error = git_checkout_head(repo, &checkout_opts);
  
    git_libgit2_shutdown();
    return error;
  }  
}
