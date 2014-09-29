#pragma once

#include "qmutex.h"
#include "qlinkedlist.h"
#include "qlist.h"
#include "qqueue.h"

/*************************************************************************
*
*	TYPES
*
*************************************************************************/

enum AgentKind {
	HostAgent,
	ClientAgent
};

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
	PNET_HEADER pHeader;
	const char * csMsg;
} NET_MSG, *PNET_MSG;

typedef QQueue<PNET_MSG> TAGENT_QUEUE;

typedef struct {
	QMutex pLockToA;
	QMutex pLockToM;
	TAGENT_QUEUE ToA;
	TAGENT_QUEUE ToM;
} AGENT_QUEUE_LIST, *PAGENT_QUEUE_LIST;

typedef QList<PAGENT_QUEUE_LIST> TAGENT_QUEUES_LIST;

typedef struct {
	QMutex pLockAgentsList;
	TAGENT_QUEUES_LIST AgentsList;
} SAFE_AGENTS_LIST, *PSAFE_AGENTS_LIST;

/*************************************************************************
*
*	FUNCTIONS
*
*************************************************************************/

inline void CNVuLL2uStr(char str[8], unsigned long long z) {
	//instead of static_cast<char> can use - 128
	str[0] = static_cast<char>(z & 0xff);
	for (register unsigned int i = 1; i < 8; i++) {
		z = (z >> 8);
		//instead of static_cast<char> can use - 128
		str[i] = static_cast<char>(z & 0xff);
	}
}
inline void CNVuStr2uLL(const char str[8], unsigned long long &c) {
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
