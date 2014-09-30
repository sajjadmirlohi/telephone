#include "Context.h"


Context::Context(AgentKind kind, QObject *parent) :
	QObject(parent)
{
	agentKind = kind;
	switch (agentKind)
	{
	case AgentKind::ClientAgent:
		pClientAgent = new AGENT_QUEUE_LIST();
		break;
	case AgentKind::HostAgent:
		pHostAgents = new SAFE_AGENTS_LIST();
		break;
	}
}


Context::~Context()
{
	if (agentKind == AgentKind::ClientAgent && pClientAgent != NULL)
	{
	}
	else if (agentKind == AgentKind::HostAgent && pHostAgents != NULL)
	{
	}
}

void Context::AddHostAgent()
{
	PAGENT_QUEUE_LIST pAgentQueueList = new AGENT_QUEUE_LIST();
	pHostAgents->AgentsList.append(pAgentQueueList);
}

void Context::MUXEnqueue(int agentID, PNET_MSG pNetMsg)
{
	if (agentKind == AgentKind::ClientAgent)
	{
		pClientAgent->ToM.enqueue(pNetMsg);
		return;
	}

	pHostAgents->AgentsList.at(agentID)->ToM.enqueue(pNetMsg);
}
bool Context::MUXDequeue(int agentID, PNET_MSG *pNetMsg)
{
	if (agentKind == AgentKind::ClientAgent)
	{
		*pNetMsg = pClientAgent->ToM.dequeue();
		return pClientAgent->ToM.size();
	}

	*pNetMsg = pHostAgents->AgentsList.at(agentID)->ToM.dequeue();
	return !(pHostAgents->AgentsList.at(agentID)->ToM.isEmpty());

}
void Context::AGENTEnqueue(int agentID, PNET_MSG pNetMsg)
{
	if (agentKind == AgentKind::ClientAgent)
	{
		pClientAgent->ToA.enqueue(pNetMsg);
		return;
	}

	pHostAgents->AgentsList.at(agentID)->ToA.enqueue(pNetMsg);
}
bool Context::AGENTDequeue(int agentID, PNET_MSG *pNetMsg)
{
	if (agentKind == AgentKind::ClientAgent)
	{
		*pNetMsg = pClientAgent->ToA.dequeue();
		return pClientAgent->ToA.size();
	}

	*pNetMsg = pHostAgents->AgentsList.at(agentID)->ToA.dequeue();
	return !(pHostAgents->AgentsList.at(agentID)->ToA.isEmpty());
}