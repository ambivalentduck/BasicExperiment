#include <QDesktopWidget>
#include <QApplication>
#include <iostream>

#include "experiment.h"
#include "getexperimentparams.h"

#define EXPERIMENT_PARAMS_PATH "/home/justin/basic experiment/"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QDesktopWidget qdw;
	std::cout << qdw.numScreens() <<" screen" << ((qdw.numScreens()-1)?"s":"\0") << " available. " << (qdw.isVirtualDesktop()?"Virutal.":"Not Virtual.")<< std::endl;
	QRect qr=qdw.screenGeometry();
	std::cout << "X:" << qr.x() << " Y:" << qr.y() << " W:" << qr.width() << " H:" << qr.height() << std::endl;
	Experiment experiment(qr, task, treatment, qdw.screen());
	experiment.start();
	QObject::connect(experiment.widget, SIGNAL(die()), &a, SLOT(quit()));
	return a.exec();
}
