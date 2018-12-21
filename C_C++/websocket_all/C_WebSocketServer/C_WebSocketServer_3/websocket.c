#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include "base64.h"
#include "sha1.h"
#include "intlib.h"
 
 
#define REQUEST_LEN_MAX         1024
#define DEFEULT_SERVER_PORT     8000
#define WEB_SOCKET_KEY_LEN_MAX  256
#define RESPONSE_HEADER_LEN_MAX 1024
#define PER_LINE_MAX            256
 
 
/**************************************************************************************
 * Function Name: extract_client_key 
 *   Description: 提取客户端发送的handshake key值
 *    Input Args: @buffer 客户端发送的握手数据
 *   Output Args: 输出客户端发来handshake key
 *  Return Value: server_key 客户端发来的handshake key
 *************************************************************************************/
static char *extract_client_key(const char * buffer)
{
    char    *key = NULL;
    char    *start = NULL; /* 要提取字符串的起始地址 */
    char    *flag = "Sec-WebSocket-Key: ";
    int     i = 0;
    int     buf_len = 0;
 
    if(NULL == buffer) {
        printf("buffer is NULL.\n");
        return NULL;
    }
 
    key=(char *)malloc(WEB_SOCKET_KEY_LEN_MAX);
    if (NULL == key) {
        printf("key alloc failure.\n");
        return NULL;
    }
    memset(key,0, WEB_SOCKET_KEY_LEN_MAX);
 
 
    start = strstr(buffer, flag);
    if(NULL == start) {
        printf("start is NULL.\n");
        return NULL;
    }
 
    start += strlen(flag);
    buf_len = strlen(buffer);
    for(i=0;i<buf_len;i++) {
        if(start[i]==0x0A || start[i]==0x0D) /* 回车换行标志 */
            break;
        key[i] = start[i];
    }
 
    return key;
} /* ----- End of extract_client_key()  ----- */
 
 
/**************************************************************************************
 * Function Name: calculate_accept_key
 *   Description: 计算handshake key值
 *    Input Args: @buffer 客户端发送的握手数据
 *   Output Args: 输出服务器发送的handshake key
 *  Return Value: server_key 服务器发送的handshake key
 *************************************************************************************/
static char *calculate_accept_key(const char * buffer)
{
    int         i;
    int         n;
    char        *client_key = NULL;
    char        *server_key = NULL; 
    char        *sha1_data_tmp = NULL;
    char        *sha1_data = NULL;
    const char  *guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; /* "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" 存放于.rodata段 */
 
 
    if(NULL == buffer) {
        printf("buffer is NULL.\n");
        return NULL;
    }
#if 0
    client_key = (char *)malloc(PER_LINE_MAX);
    if (NULL == client_key) {
        printf("client_key alloc failure.\n");
        return NULL;
    }
 
    memset(client_key, 0, PER_LINE_MAX);
#endif
    client_key = extract_client_key(buffer);
    if(NULL == client_key) {
        printf("client_key is NULL.");
        return NULL;
    }
 
    strcat(client_key, guid); /* 构建新key */
 
    sha1_data_tmp = sha1_hash(client_key); /* hash 加密 */
    n = strlen(sha1_data_tmp);
 
    sha1_data=(char *)malloc(n/2+1);
    if (NULL == sha1_data) {
        printf("sha1_data alloc failure.\n");
        return NULL;
    }
    memset(sha1_data,0,n/2+1);
 
    for(i=0; i<n; i+=2)
        sha1_data[i/2] = htoi(sha1_data_tmp, i, 2);    
 
    server_key = base64_encode(sha1_data, strlen(sha1_data)); 
 
    if (client_key != NULL) {
        free(client_key);
        client_key = NULL;
    }
 
    if (sha1_data != NULL) {
        free(sha1_data);
        sha1_data = NULL;
    }
 
    if (sha1_data_tmp != NULL) {
        free(sha1_data_tmp);
        sha1_data_tmp = NULL;
    }
 
    return server_key;
} /* ----- End of calculate_accept_key()  ----- */
 
/**************************************************************************************
 * Function Name: websocket_shakehand
 *   Description: 服务器发送握手消息
 *    Input Args: @conn_fd 连接句柄
 *                @server_key 服务器加密后的handshake key
 *   Output Args: 无
 *  Return Value: 无
 *************************************************************************************/
static void websocket_shakehand(int conn_fd, const char *server_key)
{
    char response_header[RESPONSE_HEADER_LEN_MAX];
 
    if(!conn_fd) {
        printf("connfd is error.\n");
        return ;
    }
 
    if(NULL == server_key) {
        printf("server_key is NULL.\n");
        return ;
    }
 
    memset(response_header,'\0',RESPONSE_HEADER_LEN_MAX);
 
    sprintf(response_header, "HTTP/1.1 101 Switching Protocols\r\n");
    sprintf(response_header, "%sUpgrade: websocket\r\n", response_header);
    sprintf(response_header, "%sConnection: Upgrade\r\n", response_header);
    sprintf(response_header, "%sSec-WebSocket-Accept: %s\r\n\r\n", response_header, server_key);
 
    printf("Response Header:%s\n", response_header);
 
    write(conn_fd, response_header, strlen(response_header));
} /* ----- End of websocket_shakehand()  ----- */
 
/**************************************************************************************
 * Function Name: deal_data 
 *   Description: 处理客户端数据
 *    Input Args: @buffer 接收的数据
 *                @buf_len 接收的数据长度
 *   Output Args: 无
 *  Return Value: payload_data 返回负载数据
 *************************************************************************************/
char *deal_data(const char *buffer,const int buf_len)
{
    int             i = 0; 
    char            fin;
    char            mask_flag;
    char            masks[4];
    char            *payload_data = NULL;
    char            temp[8];
    unsigned long   n;
    unsigned long   payloadLen = 0;
 
    if (buf_len < 2) {
        printf("buf_len less than 2.\n");
        return NULL;
    }
 
    fin = (buffer[0] & 0x80) == 0x80; // 1bit，1表示最后一帧  
    if (!fin) {
        printf("fin is error.\n");
        return NULL;// 超过一帧暂不处理 
    }
 
    mask_flag = (buffer[1] & 0x80) == 0x80; // 是否包含掩码  
    if (!mask_flag) {
        printf("no mask.\n");
        return NULL;// 不包含掩码的暂不处理
    }
 
    payloadLen = buffer[1] & 0x7F; // 数据长度 
    if (payloadLen == 126) {      
        memcpy(masks, buffer+4, 4);      
        payloadLen =(buffer[2]&0xFF) << 8 | (buffer[3]&0xFF);  
        payload_data=(char *)malloc(payloadLen);
        memset(payload_data,0,payloadLen);
        memcpy(payload_data,buffer+8,payloadLen);
    } else if (payloadLen == 127) {
        memcpy(masks,buffer+10,4);  
        for ( i = 0; i < 8; i++)
            temp[i] = buffer[9 - i];
 
        memcpy(&n,temp,8);  
        payload_data=(char *)malloc(n); 
        memset(payload_data,0,n); 
        memcpy(payload_data,buffer+14,n);//toggle error(core dumped) if data is too long.
        payloadLen=n;    
    } else {   
        memcpy(masks,buffer+2,4);    
        payload_data=(char *)malloc(payloadLen);
        memset(payload_data,0,payloadLen);
        memcpy(payload_data,buffer+6,payloadLen); 
    }
 
    for (i = 0; i < payloadLen; i++)
        payload_data[i] = (char)(payload_data[i] ^ masks[i % 4]);
 
    printf("data(%ld):%s\n", payloadLen, payload_data);
    return payload_data;
} /* ----- End of deal_data()  ----- */
 
/**************************************************************************************
 * Function Name: construct_packet_data
 *   Description: 组建websocket数据包
 *    Input Args: @message 发送的数据
 *                @len 发送数据长度
 *   Output Args: 无
 *  Return Value: data 返回组建后的包
 *************************************************************************************/
static char *construct_packet_data(const char *message, unsigned long *len)
{
    char *data = NULL;
    unsigned long n;
 
    if (NULL == message) {
        printf("message is NULL.\n");
        return NULL;
    }
 
    n = strlen(message);
    if (n < 126) {
        data=(char *)malloc(n+2);
        if (NULL == data) {
            printf("data is NNLL.\n");
            return NULL;
        }
        memset(data,0,n+2);	 
        data[0] = 0x81;
        data[1] = n;
        memcpy(data+2,message,n);
        *len=n+2;
    } else if (n < 0xFFFF) {
        data=(char *)malloc(n+4);
        if (NULL == data) {
            printf("data is NNLL.\n");
            return NULL;
        }
        memset(data,0,n+4);
        data[0] = 0x81;
        data[1] = 126;
        data[2] = (n>>8 & 0xFF);
        data[3] = (n & 0xFF);
        memcpy(data+4,message,n);    
        *len=n+4;
    } else {
        // 暂不处理超长内容  
        *len=0;
    }
 
    return data;
} /* ----- End of construct_packet_data()  ----- */
 
/**************************************************************************************
 * Function Name: response
 *   Description: 响应客户端
 *    Input Args: @conn_fd 连接句柄
 *                @message 发送的数据
 *   Output Args: 无
 *  Return Value: 无
 *************************************************************************************/
void response(int conn_fd, const char *message)
{
    char *data = NULL;
    unsigned long n=0;
 
    if(!conn_fd) {
        printf("conn_fd is error.\n");
        return ;
    }
 
    if(NULL == message) {
        printf("message is NULL.\n");
        return ;
    }
 
    data = construct_packet_data(message, &n); 
    if(NULL == data || n <= 0)
    {
        printf("data is empty!\n");
        return ;
    } 
 
    write(conn_fd, data, n);
 
    if (NULL == data) {
        free(data);
        data = NULL;
    }
} /* ----- End of response()  ----- */
 
 
/********************************************************************************
 * Function Name:
 *   Description:
 *    Input Args:
 *   Output Args:
 *  Return Value:
 ********************************************************************************/
int main(int argc, char *argv[])
{
    int                 listen_fd;
    int                 conn_fd;
    char                buf[REQUEST_LEN_MAX];
    char                *data = NULL;
    char                str[INET_ADDRSTRLEN];
    char                *sec_websocket_key = NULL;
    int                 n;
    int                 connected = 0;//0:not connect.1:connected.
    int                 port = DEFEULT_SERVER_PORT;
    struct sockaddr_in  servaddr;
    struct sockaddr_in  cliaddr;
    socklen_t           cliaddr_len;
 
    if(argc > 1) 
        port = atoi(argv[1]);
 
    if(port<=0 || port>0xFFFF) {
        printf("Port(%d) is out of range(1-%d)\n", port, 0xFFFF);
        return -1;
    }
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
 
    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
 
    listen(listen_fd, 20);
 
    printf("Listen %d\nAccepting connections ...\n",port);
    cliaddr_len = sizeof(cliaddr);
    conn_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    printf("From %s at PORT %d\n", \
            inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
            ntohs(cliaddr.sin_port));
 
    while (1)
    {
        memset(buf, 0, REQUEST_LEN_MAX);
        n = read(conn_fd, buf, REQUEST_LEN_MAX);	
        printf("---------------------\n");
 
        if(!connected) {
            printf("read:%d\n%s\n", n, buf);
            sec_websocket_key = calculate_accept_key(buf);	
            websocket_shakehand(conn_fd, sec_websocket_key);
            if (sec_websocket_key != NULL) {
                free(sec_websocket_key);
                sec_websocket_key = NULL;
            }
            connected=1;
            continue;
        }
 
        data = deal_data(buf, n);
        response(conn_fd, data);
    }
 
    close(conn_fd);
    return 0;
} /* ----- End of main() ----- */
