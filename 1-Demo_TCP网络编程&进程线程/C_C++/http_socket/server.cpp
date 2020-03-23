#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

int main(){
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);

    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET SockTransferRoad = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

    //接收客户端发送的数据
    char szBuffer[MAXBYTE] = {0};
    // recv(SockTransferRoad, revDataBuffer, BUFSIZ, 0);
    // recv(SockTransferRoad, revData, BUFSIZ, 0);
    // recv(SockTransferRoad, revData, BUFSIZ, NULL);
    // recv(SockTransferRoad, revData, BUFSIZ, NULL);
    recv(SockTransferRoad, szBuffer, MAXBYTE, NULL);

    //向客户端发送数据
    char *str = "Hello client!";
    // send(SockTransferRoad, sendDataBuffer, BUFSIZ, 0);
    // send(SockTransferRoad, sendData, BUFSIZ, 0);
    // send(SockTransferRoad, sendData, strlen(sendData)+1, NULL);
    // send(SockTransferRoad, sendData, strlen(str)+sizeof(char), NULL);
    send(SockTransferRoad, str, strlen(str)+sizeof(char), NULL);

    //关闭套接字
    closesocket(SockTransferRoad);
    closesocket(servSock);

    //终止 DLL 的使用
    WSACleanup();

    system("pause");
    return 0;
}
