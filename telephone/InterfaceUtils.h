#pragma once

#include "stdafx.h"

class InterfaceUtils
{
public:
	InterfaceUtils();
	~InterfaceUtils();
	
	static DWORD INIT(PDWORD, const char *, const char *, const char *);
	static DWORD INITEX(PDWORD, const char *, const char *);
	static DWORD ADD(const char *, const char *);
	static DWORD CleanUP();
	static DWORD InterfaceGetIndexInternetAccessed(const char*, PDWORD);

	static DWORD INTERNET_IFACE_INDEX;
private:
	typedef struct _PROGRAM_ITEM {
		SLIST_ENTRY ItemEntry;
		ULONG NTEContext;
	} PROGRAM_ITEM, *PPROGRAM_ITEM;

	static PSLIST_ENTRY pFirstEntry, pListEntry;
	static PSLIST_HEADER pListHead;
	static PPROGRAM_ITEM pProgramItem;
	static DWORD AddEntry(ULONG);
	static DWORD INIT_LIST();
	static DWORD InterfaceSendARP(const char *);
	static DWORD InterfaceDeleteIPAddress(ULONG);
	static DWORD InterfaceAddIPAddress(const char *, const char *, DWORD, PULONG);
};

