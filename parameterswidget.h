#ifndef PARAMPROMPT_H
#define PARAMPROMPT_H

#include <QtGui>
#include <QString>
#include "../basicexperiment/point.h"

#define DEFAULT_PARAMFILE "./Data/subjects.dat"
#define DEFAULT_DATADIRECTORY "./Data/"
#define DEFAULT_DEVICE2SCREEN_RATIO 1.0
#define DEFAULT_X_METERS .45
#define DEFAULT_Y_METERS .40
#define DEFAULT_REFRESH 60

//QFormLayout to setup
//All elements visible with default values.  Ie. Experiment conditions empty, subject number 0 (ie the universal test subject) or increasingly negative numbers as 0 is occupied.
//Return an experiment params structure.  There's no reason NOT to define it here.  As long as I use qstring to return the params, this header can be nearly universal.

//Buttons/fields/etc are private owned pointers.

struct ExperimentParameters
{
	int subjectNumber;
	QString conditions;
	point screenSizeMeters;
	unsigned int screenRefreshRate;
	double device2screenRatio;	
};
	

class ParametersWidget: public QDialog
{
	Q_OBJECT
	
public:
	ParametersWidget(QWidget *parent=0);
	ExperimentParameters params;
	ExperimentParameters getParams() {return params;}
	
private:
	QString dataDirectory;
	QString conditionFile;
	
	void setDataDirectory(QString dataDir);
	void setConditionFile(QString condfile); //Set new subject# limits, default#
	
	QPushButton *conditionfilebutton;
	QPushButton *datadirectorybutton;
	QSpinBox *subjectnumbox;
	QLineEdit *conditionsedit;  //set echomode to password, set echomode to show normal on edit
	QDoubleSpinBox *screenheightmetersline;
	QDoubleSpinBox *screenwidthmetersline;
	QDoubleSpinBox *device2screenratiobox;
	QSpinBox *refreshratebox;
	QPushButton *startbutton;
	QPushButton *quitbutton;
	
public slots:
	void setSubjectNumber(int newnumber); //Retrieve subject params as appropriate AND set subj #
	void getDataDirectory(); //Set new subject# limits, default#
	void getConditionFile(); //Set new subject# limits, default#
	void setConditions(QString newcond) {params.conditions=newcond;}
	void setScreenSizeX(double x) {params.screenSizeMeters.X()=x;}
	void setScreenSizeY(double y) {params.screenSizeMeters.Y()=y;}
	void tryaccept();
};

#endif

/*	
	
	QString output, input, prior;
	QFile qfile;
	QMessageBox::StandardButton reply;
	int out;
	bool ok=false;
	
	while(!ok)
	{
		out=QInputDialog::getInt(0, "Subject # Number Selection","Subject number from consent form:",1,1,48,1, &ok);
		if((!ok)||(out<0)) return false;
		output.setNum(out);
		if(out!=1)
		{
		
		}
		//Check that present value is unused.
		qfile.setFileName(path+output+"Cont.dat");
		if(qfile.exists())
		{
			
			reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "This subject number is already used. Overwrite the old data?", QMessageBox::Yes | QMessageBox::No);
			if (reply==QMessageBox::No) {ok=false; continue;}
		}
		output=path+output;
		std::ifstream ifs("/research/EAvs2xFF/subs.dat");
		int count=0;
		std::string line;
		while(count<out)
		{
			std::getline(ifs,line);
			count++;
		}
		QRegExp rx("(\\d{1,2}) (\\d{3}) (\\d{3})");
		ifs.close();
		rx.indexIn(line.c_str());
		QStringList qsl=rx.capturedTexts();
		QStringList::const_iterator  base=qsl.begin();
		++base;
		++base;
		enhance=(*base).toInt();
		++base;
		task=(*base).toInt();
		std::cout << enhance << " " << task << std::endl;
	}
	return true;
}
*/

