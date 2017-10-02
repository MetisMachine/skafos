
#include "thread.h"

namespace Fero {
  pthread_mutex_t Thread::mutex;

  int Thread::create(void *callback, void *args) {
    int tstat   = 0;
    tstat       = pthread_create(&this->id, NULL, (void *(*)(void *))callback, args);

    if(tstat) {
        cerr << "Unable to create threads" << endl;
        return tstat;
    } else {
        cout << "Thread created." << endl;
        return 0;
    }
  }

  int Thread::join() {
    pthread_join(this->id, NULL);

    return 0;
  }

  int Thread::initMutex() {
    if(pthread_mutex_init(&Thread::mutex, NULL) < 0) {
      cerr << "Unable to create thread mutex" << endl;

      return -1;
    } else {
      cout << "Mutex created." << endl;

      return 0;
    }
  }

  int Thread::mutexLock() {
    return (
      pthread_mutex_lock(&Thread::mutex)? 0 : -1
    );
  }

  int Thread::mutexUnlock() {
    return (
      pthread_mutex_unlock(&Thread::mutex)? 0 : -1
    );
  }
}
