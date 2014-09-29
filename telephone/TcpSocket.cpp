#include "TcpSocket.h"


TcpSocket::TcpSocket(QTcpSocket *s, Context *c, QObject *parent) :
	QObject(parent)
{
	socket = s;
	context = c;
	connect((TcpServer*)parent, SIGNAL(send(QByteArray&)), this, SLOT(send(QByteArray&)));
}


TcpSocket::~TcpSocket()
{
	delete socket;
}

void TcpSocket::connected()
{

}
void TcpSocket::disconnected()
{
	this->deleteLater();
}
void TcpSocket::readyRead()
{
	socket->readAll();
}
void TcpSocket::send(QByteArray &data)
{
	socket->write(data);
	socket->flush();
}