#pragma once

#include "qobject.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "TcpSocket.h"
#include "Context.h"

class TcpServer :
	public QTcpServer
{
	Q_OBJECT
public:
	TcpServer(Context*, PNET_HEADER _pSample, PPEER_INFO peer_info, QObject * parent = 0);
	~TcpServer();
	void BroadcastToClients(QByteArray&);
private:
	Context *context;
	PNET_HEADER pSample;
public slots:
	void peerNewConnection();
signals:
		void send(QByteArray&);
};

