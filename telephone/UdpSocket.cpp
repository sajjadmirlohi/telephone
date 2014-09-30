#include "UdpSocket.h"


UdpSocket::UdpSocket(Context *c, PNET_HEADER _pSample, QHostAddress address, quint16 port, QObject *parent) :
QUdpSocket(parent)
{
	context = c;
	pSample = _pSample;
	this->bind(address, port);
	if (context != NULL)
	{
		connect(this, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	}
}


UdpSocket::~UdpSocket()
{
}

void UdpSocket::socketReadyRead()
{
	while (this->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(this->pendingDatagramSize());

		QHostAddress senderAddress;
		quint16 senderPort;

		this->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

		PNET_MSG pMsg = new NET_MSG();
		CPYHeader(&pMsg->header, pSample);

		pMsg->header.uiDestPort = this->localPort();
		pMsg->header.uiMessageType = MessageType::FORWARD;
		pMsg->header.uiSrcPort = senderPort;
		pMsg->header.uiProtocol = Protocol::UDP;

		pMsg->msg = datagram;

		context->MUXEnqueue(pMsg);
	}
	emit context->NewDataFromAgent(pSample->uiAgentID);
}