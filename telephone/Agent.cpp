#include "Agent.h"


Agent::Agent(Context *c, int id, QHostAddress &local_addr, QHostAddress &bind_addr, QObject *parent) :
	QObject (parent)
{
	context = c;
	iAgentID = id;
	localAddress = local_addr;
	bindAddress = bind_addr;
	connect(this, SIGNAL(NewDataFromMutex(int)), this, SLOT(readyRead(int)));
}


Agent::~Agent()
{
}

void Agent::BindUDPSocket(int port)
{
	PEER_INFO peer_info = {bindAddress, port};

	UdpSocket *socket = new UdpSocket(context, context->getSample(), &peer_info, this);
	udpSocketsList.insert(port, socket);
}
void Agent::ConnectToServer(int port)
{
	PEER_INFO peer_info = { localAddress, port };

	TcpSocket *socket = new TcpSocket(NULL, context, context->getSample(), &peer_info, this);
	tcpSocketsList.insert(port, socket);
}
void Agent::CreateServer(int port)
{
	PEER_INFO peer_info = { bindAddress, port };

	TcpServer *srv = new TcpServer(context, context->getSample(), &peer_info, this);
	tcpServersList.insert(port, srv);
}

void Agent::ProcessMsgFromMutex(PNET_MSG pMsg)
{
	if (pMsg->header.uiMessageType == MessageType::FORWARD)
	{
		if (pMsg->header.uiProtocol == Protocol::UDP)
		{
			if (udpSocketsList.contains(pMsg->header.uiSrcPort))
			{
				udpSocketsList[pMsg->header.uiDestPort]->writeDatagram(pMsg->msg, localAddress, pMsg->header.uiDestPort);
			}
		}
		else if (pMsg->header.uiProtocol == Protocol::TCP)
		{
			if (pMsg->header.uiSrcPort == 0)
			{
				if (tcpSocketsList.contains(pMsg->header.uiDestPort))
				{
					tcpSocketsList[pMsg->header.uiDestPort]->write(pMsg->msg);
				}
			}
			else if (pMsg->header.uiDestPort == 0)
			{
				if (tcpServersList.contains(pMsg->header.uiSrcPort))
				{
					emit tcpServersList[pMsg->header.uiSrcPort]->send(pMsg->msg);
				}
			}
		}
	}
	else if (pMsg->header.uiMessageType == MessageType::PORT_INFO)
	{
		foreach(QString strPort, QString(pMsg->msg).split(':'))
		{
			bool conversionOK;
			quint16 port = strPort.toUShort(&conversionOK, 10);

			if (!conversionOK)
			{
				continue;
			}
			if (pMsg->header.uiProtocol == Protocol::UDP)
			{
				if (!udpSocketsList.contains(port))
				{
					BindUDPSocket(port);
				}
			}
			else if (pMsg->header.uiProtocol == Protocol::TCP)
			{
				if (!tcpServersList.contains(port))
				{
					CreateServer(port);
				}
			}
		}
	}
}

void Agent::readyRead()
{
	while (true)
	{
		PNET_MSG pMsg;
		bool moreData = context->AGENTDequeue(&pMsg, iAgentID);
		ProcessMsgFromMutex(pMsg);
		if (!moreData)
		{
			break;
		}
	}
}