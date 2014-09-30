#pragma once

#include "qobject.h"
#include "Context.h"
#include "qudpsocket.h"

class UdpSocket :
	public QUdpSocket
{
	Q_OBJECT
public:
	UdpSocket(Context *c, PNET_HEADER _pSample, PPEER_INFO peer_info, QObject *parent = 0);
	~UdpSocket();
private:
	Context *context;
	PNET_HEADER pSample;
public slots:
	void socketReadyRead();
};

