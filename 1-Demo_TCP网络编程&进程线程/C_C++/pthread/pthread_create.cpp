#include "pthread_create.h"    

namespace nutshell {

void* Hello::say_hello( void* args )    
{    
    cout << "hello...123" << endl;    
}    
    
int main()    
{    
    pthread_t tids[NUM_THREADS];    
    for( int i = 0; i < NUM_THREADS; ++i )    
    {    
        int ret = pthread_create( &tids[i], NULL, Hello::say_hello, NULL );    
        if( ret != 0 )    
        {    
            cout << "pthread_create error:error_code" << ret << endl;    
        }    
    }    
    pthread_exit( NULL );    
} 
}   

