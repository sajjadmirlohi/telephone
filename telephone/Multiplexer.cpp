#include "Multiplexer.h"


Multiplexer::Multiplexer(AgentKind kind, QObject *parent) :
QObject(parent)
{
	context = new Context(kind);
	connect(context, SIGNAL(NewDataFromAgent(int)), this, SLOT(readyReadFromAgent(int)));

	pApipa = new Apipa();
	pApipa->init();

	PEER_INFO peer_info = { pApipa->getLocalAddress(), TUNNEL_PORT };

	socket = new UdpSocket(NULL, NULL, &peer_info, this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	if (kind == AgentKind::HostAgent)
	{
		for (quint8 i = 0; i < ADDRESS_POOL_MAX; i++)
		{
			clients.append(NULL);
		}
	}
	else
	{
		host = NULL;
	}

}


Multiplexer::~Multiplexer()
{
	delete pApipa;
	socket->close();
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
	//socket->writeDatagram(PACK(pNetMsg), addr, port);
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
		if (pMsg->header.uiMessageType == MessageType::REG_2)
		{
			//process client registration_2
			quint32 agentID = pMsg->header.uiAgentID;
			if (agentID < 0 || agentID >= ADDRESS_POOL_MAX)
			{
				continue;
			}
			if (pMsg->header.uiSecret != secret)
			{
				continue;
			}
			if (clients[agentID] == NULL)
			{
				PPEER_INFO peer_info = new PEER_INFO();
				clients[agentID] = peer_info;
			}
			continue;
		}
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