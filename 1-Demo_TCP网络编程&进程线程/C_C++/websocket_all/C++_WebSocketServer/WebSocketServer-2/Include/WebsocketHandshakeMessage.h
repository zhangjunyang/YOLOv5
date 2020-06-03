#ifndef WEBSOCKETHANDSHAKEMESSAGE_H_
#define WEBSOCKETHANDSHAKEMESSAGE_H_	1
#include "WSCommon.h"
#include <string>
#include <map>

namespace ws
{
	class WS_API WebsocketHandshakeMessage
	{
	public:
		WebsocketHandshakeMessage(char* pRaw, int nSize);
		WebsocketHandshakeMessage();
		~WebsocketHandshakeMessage(void);

	public:
		bool Parse();

		std::string GetField(std::string key);
		void SetField(std::string key, std::string val);


		std::string Serialize();

	private:
		char* pRaw;
		int nSize;
		std::string m_method;
		std::string m_uri;
		std::string m_version;
		std::map<std::string, std::string> fields;
	};
	class WS_API WebSocketUtil
	{
	public:
		static bool ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response );
	};
}

#endif