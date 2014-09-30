#include "Multiplexer.h"


Multiplexer::Multiplexer(AgentKind kind, QObject *parent) :
QObject(parent)
{
	switch (kind)
	{
	case AgentKind::HostAgent:
		context = new Context(AgentKind::HostAgent);
		break;
	case AgentKind::ClientAgent:
		context = new Context(AgentKind::ClientAgent);
		break;
	}
	connect(context, SIGNAL(NewDataFromAgent(int)), this, SLOT(readyReadFromAgent(int)));
	socket = new UdpSocket(NULL, NULL, apipa.getNewAddress(), apipa.getNewPort(), this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


Multiplexer::~Multiplexer()
{
	delete socket;
	delete context;
}

void Multiplexer::CreateHostAgent()
{
	Agent *pAgent = new Agent(context, 0, this);
	agentsList.append(pAgent);
}
void Multiplexer::CreateClientAgent()
{
	Agent *pAgent = new Agent(context, 0, this);
	agentsList.append(pAgent);
}

void Multiplexer::readyReadFromAgent(int forAgentID)
{
	PNET_MSG pNetMsg;
	context->MUXDequeue(&pNetMsg);
}

void Multiplexer::readyRead()
{
	int agentID = -1;
	while (socket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());

		QHostAddress senderAddress;
		quint16 senderPort;

		socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

		PNET_MSG pMsg = UNPACK(datagram);
		context->AGENTEnqueue(pMsg, pMsg->header.uiAgentID);
		//enqueue as many as possible for the agent then emit
		if (agentID != -1)
		{
			if (agentID != pMsg->header.uiAgentID)
			{
				emit agentsList[agentID]->NewDataFromMutex();
				agentID = pMsg->header.uiAgentID;
			}
		}
		else
		{
			agentID = pMsg->header.uiAgentID;
		}
	}
	emit agentsList[agentID]->NewDataFromMutex();
}