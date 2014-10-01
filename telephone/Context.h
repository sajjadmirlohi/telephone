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
	PSAFE_AGENTS_LIST pAgents = NULL;
public:
	void AddHostAgent();
	void MUXEnqueue(PNET_MSG);
	bool MUXDequeue(PNET_MSG*);
	void AGENTEnqueue(PNET_MSG, int agentID = 0);
	bool AGENTDequeue(PNET_MSG*, int agentID = 0);
	void setSample(PNET_HEADER);
	void getSample(PNET_HEADER*);
	PNET_HEADER getSample();
signals:
	void NewDataFromAgent(int fromAgentID);
private:
	AgentKind agentKind;
	void CreateOneClientAgent();
	NET_HEADER sample;
};

