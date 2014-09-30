
#include <QtCore/QCoreApplication>
#include "Multiplexer.h"
#include "qthread.h"
#include "Types.h"
#include "NetStatus.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	NetStatus::GiveFirewallAccess(QCoreApplication::applicationFilePath().replace('/','\\'));
	//qDebug() << QCoreApplication::applicationFilePath().replace('/','\\');
	QThread muxThread;
	Multiplexer *mux = new Multiplexer(AgentKind::ClientAgent, 0);
	mux->moveToThread(&muxThread);
	a.connect(&muxThread, &QThread::finished, mux, &QObject::deleteLater);
	a.connect(&a, &QCoreApplication::aboutToQuit, &muxThread, &QThread::quit);
	muxThread.start(QThread::TimeCriticalPriority);
	Sleep(1000);
	muxThread.quit();
	return a.exec();
}
