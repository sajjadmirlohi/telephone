#pragma once

#include "qobject.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "TcpSocket.h"
#include "Context.h"

class TcpServer :
	public QObject
{
	Q_OBJECT
public:
	TcpServer(Context*, QHostAddress&, int, QObject * parent = 0);
	~TcpServer();
	void BroadcastToClients(QByteArray&);
private:
	Context *context;
	QTcpServer server;
public slots:
	void newConnection();
signals:
		void send(QByteArray&);
};

