#pragma once

#include "qobject.h"
#include "qtcpsocket.h"
#include "TcpServer.h"
#include "Context.h"

class TcpSocket :
	public QTcpSocket
{
	Q_OBJECT
public:
	TcpSocket(qintptr, Context*, PNET_HEADER _pSample, PPEER_INFO peer_info, QObject * parent = 0);
	~TcpSocket();
private:
	Context *context;
	PNET_HEADER pSample;
	qintptr descriptor;
public slots:
	void connected();
	void disconnected();
	void send(QByteArray&);
	void socketReadyRead();
};

