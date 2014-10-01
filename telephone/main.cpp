
#include <QtCore/QCoreApplication>
#include "Multiplexer.h"
#include "qthread.h"
#include "Types.h"
#include "NetStatus.h"

int main(int argc, char *argv[])
{
	/*
	-----------------
	| ATTENTION !!! |
	-----------------
	check that all sub-members defined in multiplexer and their sub-members, who have signal/slot functionality, are set with a parent
	othersise they would fallback to main thread, not the thread that created them!!!

	QObjects that are being moved must have a parent of 0.

	these are the drawbacks of moveToThread :)

	you can get rid of it by inheriting QThread from Multiplxer, and calling exec() from run()
	*/

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
