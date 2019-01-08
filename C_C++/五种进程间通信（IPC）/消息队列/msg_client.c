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
    if ((key = ftok(MSG_FILE, 'z')) < 0) 
    {
        perror("ftok error");
        exit(1);
    }

    // 打印key值
    printf("Message Queue - Client key is: %d.\n", key);

    // 打开消息队列
    if ((msqid = msgget(key, IPC_CREAT|0777)) == -1) 
    {
        perror("msgget error");
        exit(1);
    }

    // 打印消息队列ID及进程ID
    printf("My msqid is: %d.\n", msqid);
    printf("My pid is: %d.\n", getpid());

    // 添加消息，类型为888
    msg.mtype = 888;
    sprintf(msg.mtext, "hello, I'm client %d", getpid());
    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

    // 读取类型为777的消息
    msgrcv(msqid, &msg, 256, 999, 0);
    printf("Client: receive msg.mtext is: %s.\n", msg.mtext);
    printf("Client: receive msg.mtype is: %d.\n", msg.mtype);
    return 0;
}



// 发送模块 msgsend.c 的代码如下
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <sys/msg.h>
// #include <errno.h>
 
// #define MAX_TEXT 512
// struct msg_st
// {
// 	long int msg_type;
// 	char text[MAX_TEXT];
// };
 
// int main()
// {
// 	int running = 1;
// 	struct msg_st data;
// 	char buffer[BUFSIZ];
// 	int msgid = -1;
// 	int len;
 
// 	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
// 	if(msgid == -1)
// 	{
// 		fprintf(stderr, "msgget failed with error: %d\n", errno);
// 		exit(EXIT_FAILURE);
// 	}
 
// 	while(running)
// 	{
		
// 		printf("Enter data : ");
// 		fgets(buffer, BUFSIZ, stdin);
// 		data.msg_type = 5;    
// 		strcpy(data.text, buffer);
// 	    len = strlen(data.text);	
// 		if(msgsnd(msgid, (void*)&data, len-1, 0) == -1)
// 		{
// 			fprintf(stderr, "msgsnd failed\n");
// 			exit(EXIT_FAILURE);
// 		}
		
// 		if(strncmp(buffer, "end", 3) == 0)
// 			running = 0;
// 		usleep(100000);
// 	}
// 	exit(EXIT_SUCCESS);
// }
