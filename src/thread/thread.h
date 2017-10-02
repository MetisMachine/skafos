
#pragma once

#include <pthread.h>
#include "../common.h"

using namespace std;

namespace Fero {
  class Thread {
  public:
    pthread_t id;

  private:
    static pthread_mutex_t mutex;

  public:
    Thread();
    int create(void *callback, void *args);
    int join();

    static int initMutex();
    static int mutexLock();
    static int mutexUnlock();
  };
}
