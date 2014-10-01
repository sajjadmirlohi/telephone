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
	QList<Agent*> agentsList;
	QList<PPEER_INFO> clients;
	PPEER_INFO host;
	quint32 secret;
public:
	void CreateHostAgent();
	void CreateClientAgent();
public slots:
	void readyReadFromAgent(int);
	void readyRead();
};

