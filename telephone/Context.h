#pragma once

#include "qobject.h"
#include "Types.h"

class Context :
	public QObject
{
	Q_OBJECT
public:
	Context(AgentKind, QObject *parent = 0);
	~Context();
public:
	PSAFE_AGENTS_LIST pHostAgents = NULL;
	PAGENT_QUEUE_LIST pClientAgent = NULL;
public:
	void AddHostAgent();
	void MUXEnqueue(int agentID, PNET_MSG);
	int MUXDequeue(int agentID, PNET_MSG*);
	void AGENTEnqueue(int agentID, PNET_MSG);
	int AGENTDequeue(int agentID, PNET_MSG*);
signals:
	void NewDataFromAgent(int fromAgentID);
	void NewDataFromMutex(int toAgentID);
private:
	AgentKind agentKind;
	void CreateOneClientAgent();
};

