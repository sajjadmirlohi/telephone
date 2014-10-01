#include "Multiplexer.h"


Multiplexer::Multiplexer(AgentKind kind, QObject *parent) :
QObject(parent)
{
	agentKind = kind;
	context = new Context(kind);
	pid = -1;
	connect(context, SIGNAL(NewDataFromAgent(int)), this, SLOT(readyReadFromAgent(int)));

	pApipa = new Apipa();
	pApipa->init();

	localAddress = pApipa->getLocalAddress();
	PEER_INFO peer_info = { localAddress, TUNNEL_PORT };

	socket = new UdpSocket(NULL, NULL, &peer_info, this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	if (kind == AgentKind::HostAgent)
	{
		for (quint8 i = 0; i < ADDRESS_POOL_MAX; i++)
		{
			clients.insert(-1, NULL);
		}
	}
	else
	{
		host = NULL;
	}
	connect(&portFinderTimer, SIGNAL(timeout()), this, SLOT(findPorts()));
	portFinderTimer.setSingleShot(false);
	//to-do: move timer to a new thread
	//to-do: start the timer when connected to host (client side), or when host is up (server side)
	//portFinderTimer.start(10000);
}


Multiplexer::~Multiplexer()
{
	portFinderTimer.stop();
	delete pApipa;
	socket->close();
	delete socket;
	delete context;
}

void Multiplexer::CreateHostAgent(int agentID)
{
	Agent *pAgent = new Agent(context, agentID, localAddress, pApipa->getNewAddress(), this);
	agentsList.insert(agentID, pAgent);
}
void Multiplexer::CreateClientAgent()
{
	Agent *pAgent = new Agent(context, 0, localAddress, pApipa->getNewAddress(), this);
	agentsList.insert(0, pAgent);
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

void Multiplexer::findPorts()
{
	if (pid == -1)
	{
		if ((pid = NetStatus::GetProcessPIDByName(exeName)) == -1)
		{
			return;
		}
	}
	ADDR_INFO_LIST *udpAddrList = NetStatus::GetBoundedUDPList(pid);
	ADDR_INFO_LIST *tcpAddrList = NetStatus::GetListeningTCPList(pid);

	bool newPorts = false;
	if (udpAddrList != NULL)
	{
		foreach(PLOCAL_ADDR_INFO pla, *udpAddrList)
		{
			if (!udpPortsListLookup.contains(pla->localPort))
			{
				udpPortsListLookup.insert(pla->localPort, pla->localPort);
				udpPortsList.append(QString::number(pla->localPort));
				newPorts = true;
			}
		}
		if (newPorts)
		{
			QByteArray _ports = udpPortsList.join(':').toLatin1();
			if (agentKind == AgentKind::HostAgent)
			{
				foreach(PPEER_INFO peer_info, clients)
				{
					if (peer_info == NULL)
						continue;
					socket->writeDatagram(_ports, peer_info->address, peer_info->port);
				}
			}
			else
			{
				socket->writeDatagram(_ports, host->address, host->port);
			}
		}
	}
	newPorts = false;
	if (tcpAddrList != NULL)
	{
		foreach(PLOCAL_ADDR_INFO pla, *tcpAddrList)
		{
			if (!tcpPortsListLookup.contains(pla->localPort))
			{
				tcpPortsListLookup.insert(pla->localPort, pla->localPort);
				tcpPortsList.append(QString::number(pla->localPort));
				newPorts = true;
			}
		}
	}
	if (newPorts)
	{
		QByteArray _ports = tcpPortsList.join(':').toLatin1();
		if (agentKind == AgentKind::HostAgent)
		{
			foreach(PPEER_INFO peer_info, clients)
			{
				if (peer_info == NULL)
					continue;
				socket->writeDatagram(_ports, peer_info->address, peer_info->port);
			}
		}
		else
		{
			socket->writeDatagram(_ports, host->address, host->port);
		}
	}
}