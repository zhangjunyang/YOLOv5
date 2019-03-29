/*
******************************************************************************
*描述:pthread_barrier_xxx程序示例
*Use:gcc pthread_barrier_xxx.c -lpthread
* ./a.out
*******************************************************************************
*/

#include <time.h>
#include "pthread_barrier.h"

namespace nutshell {
    
Task::Task() {}

Task::~Task() {}

void Task::Task1(void *arg)
{
    printf("Task1 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);//所有线程都被阻塞在这里
    printf("Task1 is running.\r\n");
    sleep(3);//延时3s
    pthread_exit(NULL);
}

void Task::Task2(void *arg)
{
    printf("Task2 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);//所有线程都被阻塞在这里
    printf("Task2 is running.\r\n");
    sleep(3);//延时3s
    pthread_exit(NULL);
}

int main(void)
{
    int policy,inher;
    pthread_t tid;
    pthread_attr_t attr;
    struct sched_param param;

    //初始化线程属性
    pthread_attr_init(&attr);
    pthread_barrier_init(&barrier,NULL,2 + 1);//2+1个等待

    //创建线程1
    pthread_create(&tid, &attr,Task1,NULL);

    //创建线程2
    pthread_create(&tid, &attr,Task2,NULL);
    
    printf("main process will sleep 6s.\r\n");
    sleep(6);/*等待6s后,才让线程运行*/
    pthread_barrier_wait(&barrier);//起跑枪“砰!”

    pthread_join(tid, NULL);
    pthread_barrier_destroy(&barrier);
}
}


