#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET clntsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    SOCKET SockTransferRoad = connect(clntsock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //向服务器发送数据
    char *str = "Hello server!";
    // send(SockTransferRoad, sendDataBuffer, BUFSIZ, 0);
    // send(SockTransferRoad, sendData, BUFSIZ, 0);
    // send(SockTransferRoad, sendData, strlen(sendData)+1, NULL);
    // send(SockTransferRoad, sendData, strlen(str)+sizeof(char), NULL);
    send(SockTransferRoad, str, strlen(str)+sizeof(char), NULL);

    //接收服务器发送的数据
    char szBuffer[MAXBYTE] = {0};
    // recv(SockTransferRoad, revDataBuffer, BUFSIZ, 0);
    // recv(SockTransferRoad, revData, BUFSIZ, 0);
    // recv(SockTransferRoad, revData, BUFSIZ, NULL);
    // recv(SockTransferRoad, revData, BUFSIZ, NULL);
    recv(SockTransferRoad, szBuffer, MAXBYTE, NULL);

    //输出接收到的数据
    printf("Message form server: %s\n", szBuffer);

    //关闭套接字
    closesocket(SockTransferRoad);
    closesocket(clntsock);

    //终止使用 DLL
    WSACleanup();

    system("pause");
    return 0;
}

