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