#include "NetStatus.h"

QString NetStatus::interfaceName;

NetStatus::NetStatus(QObject *parent) :
QObject(parent)
{
}


NetStatus::~NetStatus()
{
}

void NetStatus::GiveFirewallAccess(QString appPath)
{
	QString script = QString(
		"netsh advfirewall firewall delete rule name=\"Telephony\" program=\"%1\""
		" & "
		"netsh advfirewall firewall add rule name=\"Telephony\" dir=in action=allow program=\"%1\" enable=yes"
		" & "
		"netsh advfirewall firewall add rule name=\"Telephony\" dir=out action=allow program=\"%1\" enable=yes").arg(appPath);
	system(script.toStdString().c_str());
}

void NetStatus::resetInterface()
{
	QString script = QString(
		"netsh interface set interface name = \"%1\" admin = disabled"
		" & "
		"netsh interface set interface name = \"%1\" admin = enabled").arg(interfaceName);
	system(script.toStdString().c_str());
}

QHostAddress NetStatus::getInternetConnectedInterfaceAddress()
{
	QNetworkInterface x = QNetworkInterface::interfaceFromIndex(InterfaceUtils::INTERNET_IFACE_INDEX);
	interfaceName = x.humanReadableName();
	foreach(QNetworkAddressEntry qnae, x.addressEntries())
	{
		if (qnae.ip().isLoopback() || !qnae.ip().toIPv4Address()) continue;
		return qnae.ip();
	}
	return QHostAddress::Null;
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
