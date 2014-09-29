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
	connect(context, SIGNAL(NewDataFromAgent(int)), this, SLOT(readyRead(int)));
}


Multiplexer::~Multiplexer()
{
	delete context;
}

void Multiplexer::CreateHostAgent()
{
	Agent *pAgent = new Agent(context, 0, this);
}
void Multiplexer::CreateClientAgent()
{
	Agent *pAgent = new Agent(context, 0, this);
}

void Multiplexer::readyRead(int forAgentID)
{

}