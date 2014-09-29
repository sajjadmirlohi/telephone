#pragma once

#include "qobject.h"
#include "qtcpsocket.h"
#include "TcpServer.h"
#include "Context.h"

class TcpSocket :
	public QObject
{
	Q_OBJECT
public:
	TcpSocket(QTcpSocket*, Context*, QObject * parent = 0);
	~TcpSocket();
private:
	QTcpSocket *socket;
	Context *context;
public slots:
	void connected();
	void disconnected();
	void readyRead();
	void send(QByteArray&);
};

