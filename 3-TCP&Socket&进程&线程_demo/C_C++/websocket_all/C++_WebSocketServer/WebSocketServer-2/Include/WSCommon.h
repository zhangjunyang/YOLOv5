#ifndef WSCOMMON_H_
#define WSCOMMON_H_	1

namespace ws
{
#ifdef WS_EXPORTS
#define WS_API __declspec(dllexport)
#else
#define WS_API __declspec(dllimport)
#endif

}

#endif