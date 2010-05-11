#include <QDesktopWidget>
#include <QApplication>
#include <iostream>
#include <stdlib.h> //For atoi
#include "experiment.h"

#define EXPERIMENT_PARAMS_PATH "/home/justin/basic experiment/"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	int enhance, task;	
	if (argc==1) {if(!loadSubject(enhance, task, EXPERIMENT_PARAMS_PATH)) return 0;} 
	else
	{
		if (argc > 1) output=QString(argv[1]);
		if (argc > 3) {enhance=atoi(argv[2]); task=atoi(argv[3]);}
		else {enhance=-1; task=-1;}
	}
	
	QDesktopWidget qdw;
	//Talk about desktop properties, relevant on multi monitor systems
	std::cout << qdw.numScreens() <<" screen" << ((qdw.numScreens()-1)?"s":"\0") << " available. " << (qdw.isVirtualDesktop()?"Virutal.":"Not Virtual.")<< std::endl;
	QRect qr=qdw.screenGeometry();
	std::cout << "X:" << qr.x() << " Y:" << qr.y() << " W:" << qr.width() << " H:" << qr.height() << std::endl;
	Experiment experiment(qr, enhance, task, qdw.screen());
	QObject::connect(experiment.widget, SIGNAL(die()), &a, SLOT(quit()));
	return a.exec();
}
