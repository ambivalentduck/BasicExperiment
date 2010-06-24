#include <QDesktopWidget>
#include <QApplication>
#include <iostream>
#include "experiment.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QDesktopWidget qdw;
	std::cout << qdw.numScreens() <<" screen" << ((qdw.numScreens()-1)?"s":"\0") << " available. " << (qdw.isVirtualDesktop()?"Virtual.":"Not Virtual.")<< std::endl;
	QRect qr=qdw.screenGeometry();
	std::cout << "X:" << qr.x() << " Y:" << qr.y() << " W:" << qr.width() << " H:" << qr.height() << std::endl;
	Experiment experiment(qr, task, treatment, qdw.screen());
	experiment.start();
	QObject::connect(&experiment, SIGNAL(endApp()), &a, SLOT(quit()));
	return a.exec();
}
