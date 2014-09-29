#include "Agent.h"


Agent::Agent(Context *c, int id, QObject *parent) :
	QObject (parent)
{
	context = c;
	iAgentID = id;
	connect(context, SIGNAL(NewDataFromMutex(int)), this, SLOT(readyRead(int)));
	//emit c->NewDataFromAgent(2);
}


Agent::~Agent()
{
}

void Agent::BindUDPSocket(int port)
{

}
void Agent::ConnectToServer(int port)
{

}
void Agent::CreateServer(int port)
{
	//TcpServer *s = new TcpServer();
}

void Agent::readyRead(int forAgentID)
{
	if (forAgentID != iAgentID)
	{
		return;
	}
}