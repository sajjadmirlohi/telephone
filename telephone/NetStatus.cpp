#include "NetStatus.h"


NetStatus::NetStatus(QObject *parent) :
QObject(parent)
{
}


NetStatus::~NetStatus()
{
}

QHostAddress NetStatus::getInternetConnectedInterfaceAddress()
{
	QNetworkInterface x = QNetworkInterface::interfaceFromIndex(InterfaceUtils::INTERNET_IFACE_INDEX);
	return x.allAddresses()[0];
}

ADDR_INFO_LIST* NetStatus::GetListeningTCPList(DWORD pid) {
	return TransportUtils::TCP(pid);
}

ADDR_INFO_LIST* NetStatus::GetBoundedUDPList(DWORD pid) {
	return TransportUtils::UDP(pid);
}

DWORD NetStatus::GetProcessPIDByName(QString &processName) {
#ifdef UNICODE
	return TransportUtils::_GetProcessPIDByName(processName.toStdWString().c_str());
#else
	return TransportUtils::_GetProcessPIDByName(processName.toStdString().c_str());
#endif
}

DWORD NetStatus::InitializeInterfaceTools(const char * someRandomAddressOnTheInternet, const char * reservedAddress, const char * maskAddress) {
	if (InterfaceUtils::INIT(&InterfaceUtils::INTERNET_IFACE_INDEX, someRandomAddressOnTheInternet, reservedAddress, maskAddress) == NO_ERROR) {
		return NO_ERROR;
	}
	return ERROR_INVALID_PARAMETER;
}

DWORD NetStatus::InitializeInterfaceToolsEx(const char * reservedAddress, const char * maskAddress) {
	if (InterfaceUtils::INITEX(&InterfaceUtils::INTERNET_IFACE_INDEX, reservedAddress, maskAddress) == NO_ERROR) {
		return NO_ERROR;
	}
	return ERROR_INVALID_PARAMETER;
}

DWORD NetStatus::WhoHasTheInternet(const char * testAddrString)
{
	if (InterfaceUtils::InterfaceGetIndexInternetAccessed(testAddrString, &InterfaceUtils::INTERNET_IFACE_INDEX) == NO_ERROR)
	{
		return NO_ERROR;
	}
	return ERROR_INVALID_PARAMETER;
}

DWORD NetStatus::CleanUPInterfaceTools() {
	return InterfaceUtils::CleanUP();
}

DWORD NetStatus::AddIP(const char * ipAddress, const char * maskAddress) {
	if (InterfaceUtils::ADD(ipAddress, maskAddress) == NO_ERROR) {
		return NO_ERROR;
	}
	return ERROR_INVALID_PARAMETER;
}
