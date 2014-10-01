#pragma once

#include "stdafx.h"
#include "Types.h"

class TransportUtils
{
public:
	TransportUtils();
	~TransportUtils();
	static ADDR_INFO_LIST* TransportUtils::TCP(DWORD &pid);
	static ADDR_INFO_LIST* TransportUtils::UDP(DWORD &pid);
#ifdef UNICODE
	static LONG _GetProcessPIDByName(const WCHAR *);
#else
	static LONG _GetProcessPIDByName(const char *);
#endif

};

