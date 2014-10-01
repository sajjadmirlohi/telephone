#pragma once

#include "qmutex.h"
#include "qlinkedlist.h"
#include "qlist.h"
#include "qqueue.h"
#include "qhostaddress.h"

/*************************************************************************
*
*	TYPES
*
*************************************************************************/

#define ADDRESS_POOL_MAX 32
#define PORT_POOL_MAX 64
#define TUNNEL_PORT 7778

typedef struct {
	QHostAddress localAddress;
	quint16 localPort;
} LOCAL_ADDR_INFO, *PLOCAL_ADDR_INFO;

typedef QList<PLOCAL_ADDR_INFO> ADDR_INFO_LIST;

enum MessageType {
	PORT_INFO,
	REG_2,
	REG_1,
	FORWARD,
	MRESERVED_1,
	PEER_REG,
	MRESERVED_2,
	MRESERVED_3
};

enum Protocol {
	NONE,
	TCP,
	UDP,
	PRESERVED_1,
	PRESERVED_2,
	PRESERVED_3,
	PRESERVED_4,
	PRESERVED_5
};

enum AgentKind {
	HostAgent,
	ClientAgent
};// AgentType;

typedef struct {
	unsigned long long mask;
	unsigned int shift;
} SHF, *PSHF;

typedef struct {
	unsigned int uiMessageType;
	unsigned int uiProtocol;
	unsigned int uiAgentType;
	unsigned int uiSrcPort;
	unsigned int uiDestPort;
	unsigned int uiAgentID;
	unsigned int uiSecret;
	unsigned int uiSequence;
} NET_HEADER, *PNET_HEADER;

typedef struct {
	NET_HEADER header;
	QByteArray msg;
} NET_MSG, *PNET_MSG;

typedef QQueue<PNET_MSG> TAGENT_QUEUE;
typedef QQueue<PNET_MSG> TMUX_QUEUE;

typedef struct {
	QMutex pLockToA;
	TAGENT_QUEUE ToA;
} AGENT_QUEUE_LIST, *PAGENT_QUEUE_LIST;

typedef QList<PAGENT_QUEUE_LIST> TAGENT_QUEUES_LIST;

typedef struct {
	QMutex pLockAgentsList;
	QMutex pLockToM;
	TAGENT_QUEUES_LIST AgentsList;
	TMUX_QUEUE ToM;
} SAFE_AGENTS_LIST, *PSAFE_AGENTS_LIST;

typedef struct {
	QHostAddress address;
	quint16 port;
} PEER_INFO, *PPEER_INFO;

/*************************************************************************
*
*	FUNCTIONS
*
*************************************************************************/

inline void CNVuLL2Str(char str[8], unsigned long long z) {
	//instead of static_cast<char> can use - 128
	str[0] = static_cast<char>(z & 0xff);
	for (register unsigned int i = 1; i < 8; i++) {
		z = (z >> 8);
		//instead of static_cast<char> can use - 128
		str[i] = static_cast<char>(z & 0xff);
	}
}
inline void CNVStr2uLL(const char str[8], unsigned long long &c) {
	c = 0;
	for (register unsigned int i = 0; i < 8; i++) {
		//instead of static_cast<unsigned char> can use + 128
		c = (c << 8) | static_cast<unsigned long long>(static_cast<unsigned char>(str[7 - i]));
	}
}
inline void CNVHeader2uLL(unsigned long long &z, PNET_HEADER header) {
	SHF op[8] = {
		{ 0, 0 }, { 0, 3 }, { 0, 2 }, { 0, 16 },
		{ 0, 16 }, { 0, 5 }, { 0, 12 }, { 0, 7 }
	};
	z = 0;
	for (register unsigned int i = 0; i < 8; i++) {
		z = (z << op[i].shift) | static_cast<unsigned long long>(*(unsigned int *)((char *)(header)+(i)* sizeof(unsigned int)));
	}
}
inline void CNVuLL2Header(unsigned long long c, PNET_HEADER header) {
	SHF op[8] = {
		{ 0x7f, 0 }, { 0xfff, 7 }, { 0x1f, 12 }, { 0xffff, 5 },
		{ 0xffff, 16 }, { 0x3, 16 }, { 0x7, 2 }, { 0x7, 3 }
	};
	for (register unsigned int i = 0; i < 8; i++) {
		c = (c >> op[i].shift);
		*(unsigned int *)((char *)(header)+(7 - i) * sizeof(unsigned int)) = static_cast<unsigned int>(c & op[i].mask);
	}
}

inline void CPYHeader(PNET_HEADER destHeader, PNET_HEADER srcHeader)
{
	for (register unsigned int i = 0; i < 8; i++) {
		*(unsigned int *)((char *)(destHeader)+(i) * sizeof(unsigned int)) = *(unsigned int *)((char *)(srcHeader)+(i) * sizeof(unsigned int));
	}
}

inline PNET_MSG UNPACK(QByteArray &datagram)
{
	quint64 c;
	PNET_HEADER pHeader = new NET_HEADER();
	PNET_MSG pMsg = new NET_MSG();
	QByteArray header = datagram.left(8);
	QByteArray msg = datagram.right(datagram.size() - 8);
	CNVStr2uLL(header.data(), c);
	CNVuLL2Header(c, pHeader);
	pMsg->msg = msg;
	memcpy(&pMsg->header, pHeader, sizeof(NET_HEADER));
	delete pHeader;
	return pMsg;
}

inline QByteArray& PACK(PNET_MSG pMsg)
{
	quint64 c;
	char str[8] = { 0 };
	CNVHeader2uLL(c, &pMsg->header);
	CNVuLL2Str(str, c);
	return (QByteArray(str, 8) += pMsg->msg);
}