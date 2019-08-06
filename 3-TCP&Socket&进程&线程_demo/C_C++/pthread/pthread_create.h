#ifndef PTHREAD_CREATE_H_
#define PTHREAD_CREATE_H_

#ifndef __cplusplus
#error ERROR: This file requires C++ compilation. Use a .cpp suffix
#endif

#include <iostream> 
#include <pthread.h> 
#define NUM_THREADS 2        

namespace nutshell {
class Hello {    
public:    
    static void* say_hello( void* args )    
};
}
#endif
    