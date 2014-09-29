#pragma once

#include "stdafx.h"
#include "InterfaceUtils.h"
#include "TransportUtils.h"

#include "qobject.h"
#include "qhostaddress.h"
#include "qnetworkinterface.h"

class NetStatus :
	public QObject
{
	Q_OBJECT
public:
	NetStatus(QObject *parent = 0);
	~NetStatus();
	static DWORD NetStatus::InitializeInterfaceToolsEx(const char * reservedAddress, const char * maskAddress);
	static DWORD WhoHasTheInternet(const char * testAddrString);
	static ADDR_INFO_LIST* GetListeningTCPList(DWORD pid);
	static ADDR_INFO_LIST* GetBoundedUDPList(DWORD pid);
	static QHostAddress getInternetConnectedInterfaceAddress();
	static DWORD GetProcessPIDByName(QString&);
	static DWORD InitializeInterfaceTools(const char *, const char *, const char *);
	static DWORD NetStatus::CleanUPInterfaceTools();
	static DWORD AddIP(const char *, const char *);
};

