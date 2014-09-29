
#include <QtCore/QCoreApplication>
#include "Multiplexer.h"
#include "qthread.h"
#include "Types.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QThread muxThread;
	Multiplexer *mux = new Multiplexer(AgentKind::ClientAgent, 0);
	mux->moveToThread(&muxThread);
	a.connect(&muxThread, &QThread::finished, mux, &QObject::deleteLater);
	a.connect(&a, &QCoreApplication::aboutToQuit, &muxThread, &QThread::quit);
	muxThread.start(QThread::TimeCriticalPriority);
	return a.exec();
}
