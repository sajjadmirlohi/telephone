#include "Agent.h"


Agent::Agent(Context *c, int id, QObject *parent) :
	QObject (parent)
{
	context = c;
	iAgentID = id;
	connect(this, SIGNAL(NewDataFromMutex(int)), this, SLOT(readyRead(int)));
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

void Agent::readyRead()
{
}