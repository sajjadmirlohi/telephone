#pragma once

#include "qobject.h"
#include "Agent.h"
#include "Context.h"

class Multiplexer :
	public QObject
{
	Q_OBJECT
public:
	Multiplexer(AgentKind, QObject *parent);
	~Multiplexer();
private:
	Context *context;
public:
	void CreateHostAgent();
	void CreateClientAgent();
public slots:
	void readyRead(int);
};

