#include "TransportUtils.h"


TransportUtils::TransportUtils()
{
}


TransportUtils::~TransportUtils()
{
}
#ifdef UNICODE
LONG TransportUtils::_GetProcessPIDByName(const WCHAR *name) {
#else
LONG TransportUtils::_GetProcessPIDByName(const char *name) {
#endif

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {

		while (Process32Next(snapshot, &entry) == TRUE) {
#ifdef UNICODE
			if (wcsicmp(entry.szExeFile, name) == 0) {
				return entry.th32ProcessID;
			}
#else
			if (_stricmp(entry.szExeFile, name) == 0) {
				return entry.th32ProcessID;
			}
#endif

		}

	}

	return -1;

}

ADDR_INFO_LIST* TransportUtils::UDP(DWORD &pid) {

	PMIB_UDPTABLE_OWNER_PID pUdpTable;
	DWORD dwSize = 0;
	DWORD ret = 0;

	pUdpTable = (MIB_UDPTABLE_OWNER_PID *)MALLOC(sizeof(MIB_UDPTABLE_OWNER_PID));

	ret = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);

checkRetANDSize:
	if (ERROR_INSUFFICIENT_BUFFER == ret) {

		FREE(pUdpTable);
		pUdpTable = (MIB_UDPTABLE_OWNER_PID *)MALLOC(dwSize);
		ret = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);
		goto checkRetANDSize;

	}
	else if (ERROR_INVALID_PARAMETER == ret) {

		FREE(pUdpTable);
		return NULL;

	}
	else {}

	char szLocalAddr[128];
	char n[5] = { 0 };
	struct in_addr IpAddr;
	ADDR_INFO_LIST *list = new ADDR_INFO_LIST();

	for (int i = 0; i < (int)pUdpTable->dwNumEntries; i++) {

		_itoa_s(i, n, 5, 10);

		MIB_UDPROW_OWNER_PID row = pUdpTable->table[i];

		IpAddr.S_un.S_addr = (u_long)row.dwLocalAddr;

		strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
		if (row.dwOwningPid == pid)
		{
			PLOCAL_ADDR_INFO inf = new LOCAL_ADDR_INFO();
			inf->localAddress = szLocalAddr;
			inf->localPort = ntohs((u_short)row.dwLocalPort);
			list->append(inf);
		}

	}

	return list;

}

ADDR_INFO_LIST* TransportUtils::TCP(DWORD &pid) {

	PMIB_TCPTABLE_OWNER_PID pTcpTable;
	DWORD dwSize = 0;
	DWORD ret = 0;
	//QList<QString> n;

	pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(sizeof(MIB_TCPTABLE_OWNER_PID));

	ret = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0);

checkRetANDSize:
	if (ERROR_INSUFFICIENT_BUFFER == ret) {

		FREE(pTcpTable);
		pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(dwSize);
		ret = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0);
		goto checkRetANDSize;

	}
	else if (ERROR_INVALID_PARAMETER == ret) {

		FREE(pTcpTable);
		return NULL;

	}
	else {}

	char szLocalAddr[128];
	char n[5] = { 0 };
	struct in_addr IpAddr;
	ADDR_INFO_LIST *list = new ADDR_INFO_LIST();

	for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {

		_itoa_s(i, n, 5, 10);

		MIB_TCPROW_OWNER_PID row = pTcpTable->table[i];

		IpAddr.S_un.S_addr = (u_long)row.dwLocalAddr;

		strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
		if (row.dwOwningPid == pid)
		{
			PLOCAL_ADDR_INFO inf = new LOCAL_ADDR_INFO();
			inf->localAddress = szLocalAddr;
			inf->localPort = ntohs((u_short)row.dwLocalPort);
			list->append(inf);
		}

	}

	return list;

}