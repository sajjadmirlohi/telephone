#include "Context.h"


Context::Context(AgentKind kind, QObject *parent) :
	QObject(parent)
{
	agentKind = kind;
	pAgents = new SAFE_AGENTS_LIST();
	if (agentKind == AgentKind::ClientAgent)
	{
		AddHostAgent();
	}
}


Context::~Context()
{
}

void Context::AddHostAgent()
{
	if (agentKind == AgentKind::ClientAgent && !pAgents->AgentsList.isEmpty())
	{
		return;
	}
	PAGENT_QUEUE_LIST pAgentQueueList = new AGENT_QUEUE_LIST();
	pAgents->AgentsList.append(pAgentQueueList);
}

void Context::MUXEnqueue(PNET_MSG pNetMsg)
{
	pAgents->ToM.enqueue(pNetMsg);
}
bool Context::MUXDequeue(PNET_MSG* pNetMsg)
{
	*pNetMsg = pAgents->ToM.dequeue();
	return !(pAgents->ToM.isEmpty());
}
void Context::AGENTEnqueue(PNET_MSG pNetMsg, int agentID)
{
	pAgents->AgentsList.at(agentID)->ToA.enqueue(pNetMsg);
}
bool Context::AGENTDequeue(PNET_MSG *pNetMsg, int agentID)
{
	*pNetMsg = pAgents->AgentsList.at(agentID)->ToA.dequeue();
	return !(pAgents->AgentsList.at(agentID)->ToA.isEmpty());
}