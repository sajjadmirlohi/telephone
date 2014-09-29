#pragma once

#include "qobject.h"
#include "qhostaddress.h"
#include "qnetworkinterface.h"
#include "NetStatus.h"
#include "qhash.h"

#define ADDRESS_POOL_MAX 32
#define PORT_POOL_MAX 64

class Apipa :
	public QObject
{
	Q_OBJECT
public:
	Apipa(QObject *parent = 0);
	~Apipa();
	QHostAddress getLocalAddress();
	QHostAddress getNewAddress();
	u_short getNewPort();
private:
	QHash<quint32, quint32> addressPool;
	QHash<quint32, quint32> badAddressPool;
	QHash<quint16, quint16> portPool;
	quint32 nextAddress;
	quint16 nextPort;
	QString mask;
	QString reserved;
	quint32 firstAddress;
	quint32 lastAddress;
	quint16 firstPort;
	quint16 lastPort;
	inline int rand(int, int);

};

