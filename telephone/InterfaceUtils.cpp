#include "InterfaceUtils.h"


DWORD InterfaceUtils::INTERNET_IFACE_INDEX = -1;
PSLIST_ENTRY InterfaceUtils::pFirstEntry;
PSLIST_ENTRY InterfaceUtils::pListEntry;
PSLIST_HEADER InterfaceUtils::pListHead;
InterfaceUtils::PPROGRAM_ITEM InterfaceUtils::pProgramItem;


InterfaceUtils::InterfaceUtils()
{
}


InterfaceUtils::~InterfaceUtils()
{
}




DWORD InterfaceUtils::InterfaceGetIndexInternetAccessed(const char* testAddrString, PDWORD interfaceIndex) {
	//BEGIN --find main interface
	DWORD ret;
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(testAddrString);//try for google
	if ((ret = GetBestInterfaceEx((struct sockaddr*)&sa, interfaceIndex)) == NO_ERROR) {
		return NO_ERROR;
	}
	else {
		switch (ret) {
		case ERROR_CAN_NOT_COMPLETE:
			printf(" (ERROR_CAN_NOT_COMPLETE)\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf(" (ERROR_INVALID_PARAMETER)\n");
			break;
		case ERROR_NOT_SUPPORTED:
			printf(" (ERROR_NOT_SUPPORTED)\n");
			break;
		default:
			printf("\n");
			break;
		}
	}
	return -1;
	//END --find main interface
}

DWORD InterfaceUtils::InterfaceAddIPAddress(const char * requestAddrString, const char * requestMaskString, DWORD interfaceIndex, PULONG pNTEContext) {
	//BEGIN --add ip to interface
	DWORD ret;
	ULONG NTEInstance = 0;
	if ((ret = AddIPAddress(inet_addr(requestAddrString), inet_addr(requestMaskString), interfaceIndex, pNTEContext, &NTEInstance)) == NO_ERROR) {
		return NO_ERROR;
	}
	else {
		switch (ret) {
		case ERROR_DEV_NOT_EXIST:
			printf(" (ERROR_DEV_NOT_EXIST)\n");
			break;
		case ERROR_DUP_DOMAINNAME:
			printf(" (ERROR_DUP_DOMAINNAME)\n");
			break;
		case ERROR_GEN_FAILURE:
			printf(" (ERROR_GEN_FAILURE)\n");
			break;
		case ERROR_INVALID_HANDLE:
			printf(" (ERROR_INVALID_HANDLE)\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf(" (ERROR_INVALID_PARAMETER)\n");
			break;
		case ERROR_NOT_SUPPORTED:
			printf(" (ERROR_NOT_SUPPORTED)\n");
			break;
		default:
			printf("\n");
			break;
		}
	}
	return -1;
	//END --add ip to interface
}

DWORD InterfaceUtils::InterfaceDeleteIPAddress(ULONG NTEContext) {
	//BEGIN --delete ip from interface
	DWORD ret;
	if ((ret = DeleteIPAddress(NTEContext)) == NO_ERROR) {
		return NO_ERROR;
	}
	else {
		switch (ret) {
		case ERROR_ACCESS_DENIED:
			printf(" (ERROR_ACCESS_DENIED)\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf(" (ERROR_INVALID_PARAMETER)\n");
			break;
		case ERROR_NOT_SUPPORTED:
			printf(" (ERROR_NOT_SUPPORTED)\n");
			break;
		default:
			printf("\n");
			break;
		}
	}
	return -1;
	//END --delete ip from interface
}

DWORD InterfaceUtils::InterfaceSendARP(const char * requestAddrString) {
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;       /* default for src ip */
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;  /* default to length of six bytes */

	char *DestIpString = NULL;
	char *SrcIpString = NULL;

	BYTE *bPhysAddr;

	SrcIpString = "0.0.0.0";//"192.168.1.30";
	SrcIp = inet_addr(SrcIpString);
	DestIp = inet_addr(requestAddrString);

	memset(&MacAddr, 0xff, sizeof (MacAddr));

	printf("Sending ARP request for IP address: %s\n", requestAddrString);

	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

	if (dwRetVal == NO_ERROR) {
		bPhysAddr = (BYTE *)& MacAddr;
		if (PhysAddrLen) {
			for (unsigned int i = 0; i < (int)PhysAddrLen; i++) {
				if (i == (PhysAddrLen - 1))
					printf("%.2X\n", (int)bPhysAddr[i]);
				else
					printf("%.2X-", (int)bPhysAddr[i]);
			}
		}
		else {
			printf("Warning: SendArp completed successfully, but returned length=0\n");
		}

		return NO_ERROR;

	}
	else {
		printf("Error: SendArp failed with error: %d", dwRetVal);
		switch (dwRetVal) {
		case ERROR_GEN_FAILURE:
			printf(" (ERROR_GEN_FAILURE)\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf(" (ERROR_INVALID_PARAMETER)\n");
			break;
		case ERROR_INVALID_USER_BUFFER:
			printf(" (ERROR_INVALID_USER_BUFFER)\n");
			break;
		case ERROR_BAD_NET_NAME:
			printf(" (ERROR_GEN_FAILURE)\n");
			break;
		case ERROR_BUFFER_OVERFLOW:
			printf(" (ERROR_BUFFER_OVERFLOW)\n");
			break;
		case ERROR_NOT_FOUND:
			printf(" (ERROR_NOT_FOUND)\n");
			break;
		default:
			printf("\n");
			break;
		}
	}
	return -1;
}

DWORD InterfaceUtils::INIT_LIST() {
	// Initialize the list header to a MEMORY_ALLOCATION_ALIGNMENT boundary.
	pListHead = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT);
	if (NULL == pListHead)
	{
		printf("Memory allocation failed.\n");
		return -1;
	}
	InitializeSListHead(pListHead);
	return NO_ERROR;
}

DWORD InterfaceUtils::AddEntry(ULONG NTEContext) {
	pProgramItem = (PPROGRAM_ITEM)_aligned_malloc(sizeof(PROGRAM_ITEM), MEMORY_ALLOCATION_ALIGNMENT);
	if (NULL == pProgramItem)
	{
		printf("Memory allocation failed.\n");
		return -1;
	}
	pProgramItem->NTEContext = NTEContext;
	pFirstEntry = InterlockedPushEntrySList(pListHead, &(pProgramItem->ItemEntry));
	return NO_ERROR;
}

DWORD InterfaceUtils::CleanUP() {
	// This example assumes that the SLIST_ENTRY structure is the 
	// first member of the structure. If your structure does not 
	// follow this convention, you must compute the starting address 
	// of the structure before calling the free function.
	while (true) {
		pListEntry = InterlockedPopEntrySList(pListHead);
		if (NULL == pListEntry)
		{
			printf("List is empty.\n");
			break;
		}
		pProgramItem = (PPROGRAM_ITEM)pListEntry;
		InterfaceDeleteIPAddress(pProgramItem->NTEContext);
		_aligned_free(pListEntry);
	}
	pListEntry = InterlockedFlushSList(pListHead);
	pFirstEntry = InterlockedPopEntrySList(pListHead);
	if (pFirstEntry != NULL) {
		printf("Error: List is not empty.\n");
		return -1;
	}
	_aligned_free(pListHead);
	return NO_ERROR;
}

DWORD InterfaceUtils::INIT(PDWORD index, const char *testAddrString, const char *reservedAddrString, const char *reservedMaskString) {
	INIT_LIST();
	ULONG NTEContext;
	if (InterfaceGetIndexInternetAccessed(testAddrString, index) == NO_ERROR) {
		if (InterfaceAddIPAddress(reservedAddrString, reservedMaskString, *index, &NTEContext) == NO_ERROR) {
			AddEntry(NTEContext);
			return NO_ERROR;
		}
	}
	return ERROR_INVALID_ACCESS;
}

DWORD InterfaceUtils::INITEX(PDWORD index, const char *reservedAddrString, const char *reservedMaskString) {
	INIT_LIST();
	ULONG NTEContext;
	if (InterfaceAddIPAddress(reservedAddrString, reservedMaskString, *index, &NTEContext) == NO_ERROR) {
		AddEntry(NTEContext);
		return NO_ERROR;
	}
	return ERROR_INVALID_ACCESS;
}

DWORD InterfaceUtils::ADD(const char *addrString, const char *maskString) {
	ULONG NTEContext;
	printf("%d\n", FlushIpNetTable(INTERNET_IFACE_INDEX));

	BOOLEAN gotArpResponse = FALSE;
	//try 5 times
	for (unsigned int i = 0; i < 2; i++) {
		if (InterfaceSendARP(addrString) == NO_ERROR) {
			gotArpResponse = TRUE;
			break;
		}
		Sleep(500);
	}

	if (!gotArpResponse) {
		if (InterfaceAddIPAddress(addrString, maskString, INTERNET_IFACE_INDEX, &NTEContext) == NO_ERROR) {
			AddEntry(NTEContext);
			return NO_ERROR;
		}
	}
	return -1;
}