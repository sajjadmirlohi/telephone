#include "Apipa.h"


Apipa::Apipa(QObject *parent) :
QObject(parent)
{
}


Apipa::~Apipa()
{
	cleanUP();
}

void Apipa::resetInterface()
{
	NetStatus::resetInterface();
}

void Apipa::init()
{
	mask = QString("255.255.255.0");
	NetStatus::WhoHasTheInternet("4.2.2.4");
	QHostAddress haRes = getLocalAddress();
	QString res = haRes.toString();
	QStringList s = res.split('.');
	if (s[0] == "192" && s[1] == "168") {
		int o3 = s[2].toInt() + 1;
		if (o3 > 254) {
			o3 -= 3;
		}
		QString strO3 = QString::number(o3);
		reserved = QString("192.168.%1.2").arg(strO3);
		firstAddress = QHostAddress(QString("192.168.%1.3").arg(strO3)).toIPv4Address();
		lastAddress = QHostAddress(QString("192.168.%1.254").arg(strO3)).toIPv4Address();
	}
	else {
		reserved = QString("192.168.1.2");
		firstAddress = QHostAddress(QString("192.168.1.3")).toIPv4Address();
		lastAddress = QHostAddress(QString("192.168.1.254")).toIPv4Address();
	}
	firstPort = 62000;
	lastPort = 63000;
	nextAddress = firstAddress;
	nextPort = firstPort;
	NetStatus::InitializeInterfaceToolsEx(reserved.toStdString().c_str(), mask.toStdString().c_str());
}

void Apipa::cleanUP()
{
	NetStatus::CleanUPInterfaceTools();
}

QHostAddress Apipa::getLocalAddress()
{
	return NetStatus::getInternetConnectedInterfaceAddress();
}

int Apipa::rand(int min, int max)
{
	return ((((int)((double)qrand() / (double)RAND_MAX)) * (max - min + 1)) + min);
}

QHostAddress Apipa::getNewAddress()
{
	QHostAddress res = QHostAddress::Null;
	if (addressPool.size() > ADDRESS_POOL_MAX)
		return res;
	while ((addressPool.contains(nextAddress) || badAddressPool.contains(nextAddress)) && nextAddress <= lastAddress)
	{
		if (++nextAddress == lastAddress)
			return res;
	}
	if (NetStatus::AddIP(QHostAddress(nextAddress).toString().toStdString().c_str(), mask.toStdString().c_str()) != NO_ERROR)
	{
		badAddressPool.insert(nextAddress, nextAddress);
		return res;
	}
	else
	{
		addressPool.insert(nextAddress, nextAddress);
		res = QHostAddress(nextAddress);
	}

	return res;
}

quint16 Apipa::getNewPort()
{
	quint16 res = 0;
	if (portPool.size() > PORT_POOL_MAX)
		return res;
	while ((portPool.contains(nextPort)) && nextPort <= lastPort)
	{
		if (++nextPort == lastPort)
			return res;
	}
	portPool.insert(nextPort, nextPort);
	res = nextPort;
	return res;
}