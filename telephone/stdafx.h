#pragma once

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <Iphlpapi.h>
#include <Tlhelp32.h>

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#include "qlist.h"
#include "qstring.h"
#include "qhostaddress.h"

typedef struct {
	QHostAddress localAddress;
	u_short localPort;
} LOCAL_ADDR_INFO, *PLOCAL_ADDR_INFO;

typedef QList<PLOCAL_ADDR_INFO> ADDR_INFO_LIST;
