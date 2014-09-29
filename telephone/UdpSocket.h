#pragma once
#include "qobject.h"
class UdpSocket :
	public QObject
{
	Q_OBJECT
public:
	UdpSocket();
	~UdpSocket();
};

