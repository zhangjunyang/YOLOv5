#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

// 用于创建一个唯一的key
#define MSG_FILE "/etc/passwd"

// 消息结构
struct msg_form {
    long mtype;
    char mtext[256];
};

int main()
{
    int msqid;
    key_t key;
    struct msg_form msg;
    
    // 获取key值
    if((key = ftok(MSG_FILE,'z')) < 0)
    {
        perror("ftok error");
        exit(1);
    }

    // 打印key值
    printf("Message Queue - Server key is: %d.\n", key);

    // 创建消息队列
    if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    // 打印消息队列ID及进程ID
    printf("My msqid is: %d.\n", msqid);
    printf("My pid is: %d.\n", getpid());

    // 循环读取消息
    for(;;) 
    {
        msgrcv(msqid, &msg, 256, 888, 0);// 返回类型为888的第一个消息
        printf("Server: receive msg.mtext is: %s.\n", msg.mtext);
        printf("Server: receive msg.mtype is: %d.\n", msg.mtype);

        msg.mtype = 999; // 客户端接收的消息类型
        sprintf(msg.mtext, "hello, I'm server %d", getpid());
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
    }
    return 0;
}



// 接收模块 msgreceive.c 的代码如下
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <errno.h>
// #include <sys/msg.h>
 
// struct msg_st
// {
// 	long int msg_type;
// 	char text[128];
// };
 
// int main()
// {
// 	int running = 1;
// 	int msgid = -1;
// 	int len = 0;
// 	long int msgtype = 5; 
// 	struct msg_st data;
		
// 	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
// 	if(-1 == msgid )
// 	{
// 		fprintf(stderr, "msgget failed with error: %d\n", errno);
// 		exit(EXIT_FAILURE);
// 	}
	
// 	while(running)
// 	{
// 		memset(&data.text, 0, 128);
// 		len = msgrcv(msgid, (void*)&data, 128, msgtype, 0);
// 		if(-1 == len)
// 		{
// 			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
// 			exit(EXIT_FAILURE);
// 		}
// 		printf("You wrote: %s\n",data.text);
 
// 		if(0 == strncmp(data.text, "end", 3))
// 			running = 0;
// 	}
 
// 	//remove message queue
// 	if(-1 == msgctl(msgid, IPC_RMID, 0))
// 	{
// 		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	exit(EXIT_SUCCESS);
// }

// 在接收端消息队列类型中的msg_type，如果设置为0，则表示接收所有的消息。如果非零，则只接收与发送端类型一样的消息。
// 比如接收端设置为3，那么接收端只接收发送端msg_type为3进程发送过来的消息。
// 与命名管道相比，消息队列的优势在于：
// 1、消息队列也可以独立于发送和接收进程而存在，从而消除了在同步命名管道的打开和关闭时可能产生的困难。
// 2、同时通过发送消息还可以避免命名管道的同步和阻塞问题，不需要由进程自己来提供同步方法。
// 3、接收程序可以通过消息类型有选择地接收数据，而不是像命名管道中那样，只能默认地接收。
