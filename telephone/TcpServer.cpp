#include "TcpServer.h"


TcpServer::TcpServer(Context *c, PNET_HEADER _pSample, PPEER_INFO peer_info, QObject *parent) :
	QTcpServer(parent)
{
	pSample = _pSample;
	context = c;
	this->setMaxPendingConnections(1);
	connect(this, SIGNAL(newConnection()), this, SLOT(peerNewConnection()));
	this->listen(peer_info->address, peer_info->port);
}


TcpServer::~TcpServer()
{
}

void TcpServer::peerNewConnection()
{
	TcpSocket *socket = new TcpSocket(this->nextPendingConnection()->socketDescriptor(), context, pSample, NULL, this);
}

void TcpServer::BroadcastToClients(QByteArray &msg)
{
	emit send(msg);
}