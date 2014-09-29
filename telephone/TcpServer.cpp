#include "TcpServer.h"


TcpServer::TcpServer(Context *c, QHostAddress &addr, int port, QObject *parent) :
	QObject(parent)
{
	context = c;
	server.setMaxPendingConnections(1);
	connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	server.listen(addr, port);
}


TcpServer::~TcpServer()
{
}

void TcpServer::newConnection()
{
	TcpSocket *socket = new TcpSocket(server.nextPendingConnection(), context, this);
}

void TcpServer::BroadcastToClients(QByteArray &msg)
{
	emit send(msg);
}