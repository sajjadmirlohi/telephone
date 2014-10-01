#pragma once

#include "qobject.h"
#include "Context.h"
#include "TcpServer.h"
#include "UdpSocket.h"
#include "TcpSocket.h"

class Agent :
	public QObject
{
	Q_OBJECT
public:
	Agent(Context*, int, QHostAddress &local_addr, QHostAddress &bind_addr, QObject *parent = 0);
	~Agent();
private:
	Context *context;
	int iAgentID;
	void BindUDPSocket(int port);
	void ConnectToServer(int port);
	void CreateServer(int port);
	void ProcessMsgFromMutex(PNET_MSG);
	QHostAddress localAddress;
	QHostAddress bindAddress;

	QHash<quint16, UdpSocket*> udpSocketsList;
	QHash<quint16, TcpServer*> tcpServersList;
	QHash<quint16, TcpSocket*> tcpSocketsList;
signals:
	void NewDataFromMutex();
public slots:
	void readyRead();
};

