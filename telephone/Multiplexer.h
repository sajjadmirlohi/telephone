#pragma once

#include "qobject.h"
#include "Agent.h"
#include "Context.h"
#include "UdpSocket.h"
#include "Apipa.h"

class Multiplexer :
	public QObject
{
	Q_OBJECT
public:
	Multiplexer(AgentKind, QObject *parent);
	~Multiplexer();
private:
	Context *context;
	UdpSocket *socket;
	Apipa *pApipa;
	QHash<int, Agent*> agentsList;
	QHash<int, PPEER_INFO> clients;
	PPEER_INFO host;
	quint32 secret;
	QHostAddress localAddress;
public:
	void CreateHostAgent(int agentID);
	void CreateClientAgent();
public slots:
	void readyReadFromAgent(int);
	void readyRead();
};

