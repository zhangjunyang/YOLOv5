#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>//winsock

#include <windows.h>//threads
#include <process.h>

#pragma comment(lib, "wininet.lib")//winsock
#pragma comment(lib,"Ws2_32.lib")//winsock

#include <string>
#include <sstream>
#include <iomanip>
#include <list>
#include <regex>
#include "SHA1.h"
#include "DataFrame.h"


using namespace std;

HANDLE ghMutex; 
HANDLE hClientMain;

/*	COMMON	*/
const char* _host = "127.0.0.1";
const char* _port = "38950";
#define NUMCLIENTS 3
#define NUM_MSGS_PER_CLIENT 12

/*	SERVER */
char shared_buffer[BUFSIZ + 1];	//+1 for null-termination
SOCKET listenfd;					//server's 1 listening SOCKET.
HANDLE connectHandles[NUMCLIENTS];	//server handles for handling client threads.
SOCKET connectDescriptors[NUMCLIENTS];
list<SOCKET> connectList;
unsigned nClients = 0;				//current number of clients connected
void init_resources();	//initialization related to multithreading
void server_startup();
void server_dispose();
SOCKET accept_client();
unsigned __stdcall on_client_connected( void* pArguments );
BOOL isMutexEnabled = true;	//FALSE to allow race conditions.
void acquire_mutex();
void release_mutex();
int server_send(char* src, SOCKET s, bool isWebSocket = true);
int server_recv(char* buffer, SOCKET s, bool isWebSocket = true);	//assume buffer and actual framed data size < BUFSIZ
/*	SERVER HANDSHAKE	*/
bool server_negotiate_websockets_handshake(SOCKET connectfd);

/*	CLIENT	*/
unsigned threadIDs[NUMCLIENTS];		
HANDLE clientHandles[NUMCLIENTS];	//client threads (clientfd)
void open_clientfd(SOCKET & clientfd);
unsigned __stdcall client_startup_main( void* pArguments );
unsigned __stdcall client_thread( void* pArguments );
void client_handshake(SOCKET clientfd);
void client_dispose();
int client_recv_frame(char* buffer, SOCKET s);
int client_send_frame(char* src, SOCKET s);

//base64±àÂë
static const std::string base64_chars =   
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  
             "abcdefghijklmnopqrstuvwxyz"  
             "0123456789+/";  

bool is_base64(unsigned char c) {  
  return (isalnum(c) || (c == '+') || (c == '/'));  
}  
  
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {  
  std::string ret;  
  int i = 0;  
  int j = 0;  
  unsigned char char_array_3[3];  
  unsigned char char_array_4[4];  
  
  while (in_len--) {  
    char_array_3[i++] = *(bytes_to_encode++);  
    if (i == 3) {  
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
      char_array_4[3] = char_array_3[2] & 0x3f;  
  
      for(i = 0; (i <4) ; i++)  
        ret += base64_chars[char_array_4[i]];  
      i = 0;  
    }  
  }  
  
  if (i)  
  {  
    for(j = i; j < 3; j++)  
      char_array_3[j] = '\0';  
  
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
    char_array_4[3] = char_array_3[2] & 0x3f;  
  
    for (j = 0; (j < i + 1); j++)  
      ret += base64_chars[char_array_4[j]];  
  
    while((i++ < 3))  
      ret += '=';
  }
  return ret;
}  
  
std::string base64_decode(std::string const& encoded_string) {  
  int in_len = encoded_string.size();  
  int i = 0;  
  int j = 0;  
  int in_ = 0;  
  unsigned char char_array_4[4], char_array_3[3];  
  std::string ret;  
  
  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {  
    char_array_4[i++] = encoded_string[in_]; in_++;  
    if (i ==4) {  
      for (i = 0; i <4; i++)  
        char_array_4[i] = base64_chars.find(char_array_4[i]);  
  
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  
  
      for (i = 0; (i < 3); i++)  
        ret += char_array_3[i];  
      i = 0;  
    }  
  }  
  
  if (i) {  
    for (j = i; j <4; j++)  
      char_array_4[j] = 0;  
  
    for (j = 0; j <4; j++)  
      char_array_4[j] = base64_chars.find(char_array_4[j]);  
  
    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  
  
    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];  
  }  
  
  return ret;  
} 

///////////////

#ifdef _UNICODE
typedef std::wstring tstring;
#define tcout std::wcout
#define tcin std::wcin
#else
typedef std::string tstring;
#define tcout std::cout
#define tcin std::cin
#endif
#include <locale.h> 

std::string WstringToString(const std::wstring str)
{// wstring×ªstring
    unsigned len = str.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p,str.c_str(),len);
    std::string str1(p);
    delete[] p;
    return str1;
}

std::wstring stringToWstring(const std::string& str)
{
    LPCSTR pszSrc = str.c_str();
    int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
    if (nLen == 0) 
        return std::wstring(L"");

    wchar_t* pwszDst = new wchar_t[nLen];
    if (!pwszDst) 
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
    std::wstring wstr(pwszDst);
    delete[] pwszDst;
    pwszDst = NULL;

    return wstr;
}

char* HashString(string szMsg)
{
	tstring str = stringToWstring(szMsg);

	CSHA1 sha1;
	tstring strReport;

#ifdef _UNICODE
	const size_t uAnsiLen = wcstombs(NULL, str.c_str(), 0) + 1;
	char* pszAnsi = new char[uAnsiLen + 1];
	wcstombs(pszAnsi, str.c_str(), uAnsiLen);

	sha1.Update((UINT_8*)&pszAnsi[0], strlen(&pszAnsi[0]));
	sha1.Final();
	char* szHash = new char[20];
	//sha1.ReportHashStl(strReport, szHash, CSHA1::REPORT_HEX_SHORT);
	for(int i = 0; i < 20; ++i)
	{
		szHash[i] = sha1.m_digest[i];
	}

	delete[] pszAnsi;
	sha1.Reset();

#else
	sha1.Update((UINT_8*)str.c_str(), str.size() * sizeof(TCHAR));
	sha1.Final();
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	tcout << _T("String hashed to:") << endl;
	tcout << strReport << endl;
#endif
	return szHash;
}


int main() 
{
	init_resources();
	server_startup();
	
	while (true)
	{
		SOCKET connectfd = accept_client();
		connectDescriptors[nClients] = connectfd;				
		connectList.push_back(connectfd);
		unsigned *threadid = (unsigned*)malloc(sizeof(int));
		connectHandles[nClients] = (HANDLE)_beginthreadex(NULL, 0, 
																		&on_client_connected, 
																		&connectDescriptors[nClients], //connectfd
																		0, threadid);
																	nClients += 1;
		//closesocket(connectfd);	//do not do this (even though Bryant + O'Hallaron say to)
	}
	server_dispose();
	return 0;
}

unsigned __stdcall on_client_connected( void* pArguments ) {
	SOCKET connectfd = *(SOCKET*)pArguments;
	bool isWebSocket = server_negotiate_websockets_handshake(connectfd);
	char* local_buffer = (char*)calloc(BUFSIZ, sizeof(char));
	int received;
	do {
		memset(local_buffer, '\0', BUFSIZ);
		received = server_recv(local_buffer, connectfd, isWebSocket);
		while(1)
		{
			if((received = server_recv(local_buffer, connectfd, isWebSocket)) > 0)
			{
				break;
			}
		}
		
		acquire_mutex();		
		memcpy(shared_buffer, local_buffer, BUFSIZ);
		printf("server thread %x\tshared buffer: \"%s\"\n", GetCurrentThreadId(), shared_buffer);
		//server_send(shared_buffer, connectfd, isWebSocket);
		release_mutex();
		server_send(local_buffer, connectfd, isWebSocket);
	} while (received > 0);	//continue until client SOCKET closes.
	closesocket(connectfd);
	connectList.remove(connectfd);
	printf("Client socket %u connection closed.\nExiting server thread %x...\n", GetCurrentThread(), connectfd);
	return 0;
}

/*	returns TRUE if this is a websockets client.
	only need to check this once (after accept).
*/
bool server_negotiate_websockets_handshake(SOCKET connectfd) {	
	const char *key1 = NULL, *origin = NULL, *resource = NULL;
	char *location = (char*)calloc(0x40, sizeof(char));
	char requestbuf[BUFSIZ];
	unsigned char responsebuf[BUFSIZ];
	memset(requestbuf,0,BUFSIZ);
	memset(responsebuf,0,BUFSIZ);

	int received = recv(connectfd, requestbuf, BUFSIZ, 0);

	char* key1pattern = "(Sec-WebSocket-Key:)[[:s:]](.+\\r\\n)";
	char* resourcePattern = "(GET)[[:s:]](/[[:alnum:]]+)";
	char* originPattern = "(Origin:)[[:s:]](.+)\\r\\n";

	match_results<const char*> m;
	tr1::regex rx;	
	string s;

	//match Sec-WebSocket-Key1 
	m = match_results<const char*>();
	rx = tr1::regex(key1pattern);
	tr1::regex_search(requestbuf, m, rx);	
	s = m[2];
	string strNew = s.substr(0, s.length() - 2);
	key1 = strdup(strNew.c_str());

	//match origin
	m = match_results<const char*>();
	rx = tr1::regex(originPattern);
	tr1::regex_search(requestbuf, m, rx);
	s = m[2];
	origin = strdup(s.c_str());

	//match GET (resource)
	m = match_results<const char*>();
	rx = tr1::regex(resourcePattern);
	tr1::regex_search(requestbuf, m, rx);
	s = m[2];
	resource = strdup(s.c_str());

	sprintf(location, "ws://%s:%s%s", _host, _port, resource);
	//ws->location = "ws://" + ws->ipAddress + ":" + ws->port + *resource;
	if (key1 == NULL || resource == NULL)// if (*key1 == "" || *key2 == "" || *resource == "")
		return false;

	//create handshake response:
	//key1 = "0z0hQ4F9M8jG5EDzgWXThQ==";
	unsigned char serv_buf[BUFSIZ];
	memset(serv_buf,0,BUFSIZ);
	char* hash;
	int n = sprintf((char *)serv_buf, "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", key1);
	string strRaw = (char *)serv_buf;
	//sha1_hash((char*)serv_buf, hash);
	//strRaw = "0z0hQ4F9M8jG5EDzgWXThQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

	hash = HashString(strRaw);

	int pt_serv_buf_size = 0;
	string strAccept = base64_encode((unsigned char*)hash, 20);
	if (strAccept.size() < 0) 
	{
		return false;
	}

	char* handshake = (char*)calloc(BUFSIZ,sizeof(char));
	char * handshakeFormat = "HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: WebSocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n"
		"\r\n";
	sprintf(handshake, handshakeFormat, strAccept.c_str());

	for (int i = 0; i < strlen(handshake); i++)
	{
		responsebuf[i] = handshake[i];
	}
	int sent = send(connectfd, (char*)responsebuf, 129, 0);
	if (sent > 0)
	{
		memset(responsebuf, 0x00, BUFSIZ);
		sprintf((char*)responsebuf, "client socket descriptor is %d.", connectfd);
		sent = server_send((char*)responsebuf, connectfd, true);
	}
	return sent > 0;
}

SOCKET accept_client() {
	struct sockaddr clientinfo = { 0 };
	SOCKET connectfd = accept(listenfd, &clientinfo, NULL);		
	struct sockaddr_in* ipv4info = (struct sockaddr_in*)&clientinfo;
	printf("Client connected on socket %x. Address %s:%d\n", connectfd, 
	inet_ntoa(ipv4info->sin_addr), ntohs(ipv4info->sin_port));		
	return connectfd;
}


/*	SERVER		*/
void server_startup() 
{
	WSADATA wsaData;	
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)// Initialize Winsock
		printf("Error at WSAStartup()\n");
	//create the SOCKET
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

	struct sockaddr_in server = { 0 };
	server.sin_family = AF_INET;	
	server.sin_addr.s_addr = inet_addr(_host);
	server.sin_port = htons((u_short)atoi(_port));
	
	// Bind the socket.
	if (bind( listenfd, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR) 
	{
		printf("Error on server bind.\n");
		return;
	}		
	if (listen(listenfd, SOMAXCONN ) == SOCKET_ERROR)
	{
		printf("Error listening on socket.\n");
		return;
	}		
	printf("Server is listening on socket... %s:%u\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));	
}


void init_resources() {
	ghMutex = CreateMutex( 
	NULL,              // default security attributes
	FALSE,             // initially not owned
	NULL);             // unnamed mutex

	if (ghMutex == NULL) 
		printf("CreateMutex error: %x\n", GetLastError());
	
}

void acquire_mutex() 
{
	if (!isMutexEnabled)	
		return;
	if ( WaitForSingleObject(ghMutex, INFINITE) != WAIT_OBJECT_0) { 
		printf("Error on WaitForSingleObject (thread %x)\n", GetCurrentThreadId()); 
	}
}

void release_mutex() 
{
	if (!isMutexEnabled)
		return;

	if (!ReleaseMutex(ghMutex)) { 
		printf("Error releasing Mutex on thread %x.\n", GetCurrentThreadId()); 
	}
}

void server_dispose() {
	CloseHandle(ghMutex);
	CloseHandle(hClientMain);
	for (int i = 0; i < NUMCLIENTS; i++) {
		closesocket(connectDescriptors[i]);
		CloseHandle(connectHandles[i]);
	}
	closesocket(listenfd);
	
}

int server_send(char* src, SOCKET s, bool isWebSocket)
{	
	if (!isWebSocket) {
		return send(s, src, strlen(src), 0);
	}
	DataFrame dr = DataFrame(src, strlen(src));

	//Block Test
	//byte* bsend = new byte[2 + strlen(src)];
	//bsend[0] = 0x81; // last frame, text
	//bsend[1] = strlen(src); // not masked, length 3
	//memcpy(bsend + 2, src, strlen(src));
	//int nSend = send(s, bsend, 2 + strlen(src), 0);

	int nsize;
	char* msg = dr.GetBytes(nsize);
	return send(s, msg, nsize, 0);
}



int server_recv(char* buffer, SOCKET s, bool isWebSocket){	

	memset(buffer, 0x00, BUFSIZ);
	if (!isWebSocket) {
		return recv(s, buffer, BUFSIZ, 0);
	}

	char framed_data[BUFSIZ];
	int received = recv(s, framed_data, BUFSIZ, 0);
	char end = 0xFF;
	for (int i = 1; framed_data[i] != end && i < received; i++)
	{
		buffer[i - 1] = framed_data[i];	
	}	
	return received;
}
