#include <QDesktopWidget>
#include <QApplication>
#include <iostream>

#include "experiment.h"

#define EXPERIMENT_PARAMS_PATH "/home/justin/basic experiment/"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	QString treatment, task;	
	if (argc==1) {if(!loadSubject(task, treatment, EXPERIMENT_PARAMS_PATH)) return 0;} 
	else
	{
		if (argc > 1) output=QString(argv[1]);
		if (argc > 2) task=argv[2]); else task="";
		if (argc > 3) treatment = argv[3]; else treatment="";
	}
	
	QDesktopWidget qdw;
	std::cout << qdw.numScreens() <<" screen" << ((qdw.numScreens()-1)?"s":"\0") << " available. " << (qdw.isVirtualDesktop()?"Virutal.":"Not Virtual.")<< std::endl;
	QRect qr=qdw.screenGeometry();
	std::cout << "X:" << qr.x() << " Y:" << qr.y() << " W:" << qr.width() << " H:" << qr.height() << std::endl;
	Experiment experiment(qr, task, treatment, qdw.screen());
	QObject::connect(experiment.widget, SIGNAL(die()), &a, SLOT(quit()));
	return a.exec();
}
