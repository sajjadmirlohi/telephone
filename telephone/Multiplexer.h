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
	Apipa apipa;
	QList<Agent*> agentsList;
public:
	void CreateHostAgent();
	void CreateClientAgent();
public slots:
	void readyReadFromAgent(int);
	void readyRead();
};

