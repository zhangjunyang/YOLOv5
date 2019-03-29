#ifndef PTHREAD_BRRIER_H_
#define PTHREAD_BRRIER_H_

#ifndef __cplusplus
#error ERROR: This file requires C++ compilation. Use a .cpp suffix
#endif

#include <pthread.h>

namespace nutshell {
class Task {
private:  //私有的
    pthread_barrier_t barrier;

public: 
    Task();
    virtual ~Task();
    void Task1(void *arg);
    void Task2(void *arg);
};
}
