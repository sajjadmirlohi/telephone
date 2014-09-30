#pragma once

#include "qobject.h"
#include "Context.h"
#include "TcpServer.h"

class Agent :
	public QObject
{
	Q_OBJECT
public:
	Agent(Context*, int, QObject *parent = 0);
	~Agent();
private:
	Context *context;
	int iAgentID;
	void BindUDPSocket(int port);
	void ConnectToServer(int port);
	void CreateServer(int port);
signals:
	void NewDataFromMutex();
public slots:
	void readyRead();
};

