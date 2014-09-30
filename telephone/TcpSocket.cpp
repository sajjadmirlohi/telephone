#include "TcpSocket.h"


TcpSocket::TcpSocket(qintptr descriptor, Context *c, PNET_HEADER _pSample, PPEER_INFO peer_info, QObject *parent) :
QTcpSocket(parent)
{
	this->descriptor = descriptor;
	if (descriptor != NULL){
		this->setSocketDescriptor(descriptor);
		connect((TcpServer*)parent, SIGNAL(send(QByteArray&)), this, SLOT(send(QByteArray&)));
	}
	else
	{
		this->connectToHost(peer_info->address, peer_info->port);
	}
	context = c;
	connect(this, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
}


TcpSocket::~TcpSocket()
{
}

void TcpSocket::connected()
{

}
void TcpSocket::disconnected()
{
	this->deleteLater();
}
void TcpSocket::send(QByteArray &data)
{
	this->write(data);
	this->flush();
}
void TcpSocket::socketReadyRead()
{
	QByteArray data = this->readAll();
	PNET_MSG pMsg = new NET_MSG();
	CPYHeader(&pMsg->header, pSample);

	pMsg->header.uiMessageType = MessageType::FORWARD;
	pMsg->header.uiProtocol = Protocol::TCP;
	
	if (descriptor == NULL)
	{
		//connected to remote server, remote server port
		pMsg->header.uiSrcPort = this->peerPort();
		pMsg->header.uiDestPort = 0;
	}
	else
	{
		//connection to this server
		pMsg->header.uiSrcPort = 0;
		//our server's port
		pMsg->header.uiDestPort = this->localPort();
	}

	pMsg->msg = data;

	context->MUXEnqueue(pMsg);
	emit context->NewDataFromAgent(pSample->uiAgentID);
}