#include "parameterswidget.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QStringList>

ParametersWidget::ParametersWidget(QWidget *parent): QDialog(parent)
{
	setWindowTitle(tr("Experiment Conditions And Settings"));
	subjectnumbox=new QSpinBox;
	conditionsedit=new QLineEdit;
	datadirectorybutton=new QPushButton;
	conditionfilebutton=new QPushButton;
	device2screenratiobox=new QDoubleSpinBox;
	screenheightmetersline=new QDoubleSpinBox;
	screenwidthmetersline=new QDoubleSpinBox;
	refreshratebox=new QSpinBox;
	startbutton=new QPushButton(tr("Start"));
	quitbutton=new QPushButton(tr("Quit"));

	device2screenratiobox->setValue(DEFAULT_DEVICE2SCREEN_RATIO);
	screenheightmetersline->setValue(DEFAULT_Y_METERS);
	screenwidthmetersline->setValue(DEFAULT_X_METERS);
	refreshratebox->setValue(DEFAULT_REFRESH);
	
	setConditionFile(tr(DEFAULT_PARAMFILE));
	setDataDirectory(tr(DEFAULT_DATADIRECTORY));

	connect(quitbutton, SIGNAL(clicked()),this,SLOT(reject()));
	connect(startbutton,SIGNAL(clicked()),this,SLOT(tryaccept()));
	connect(subjectnumbox,SIGNAL(valueChanged(int)),this,SLOT(setSubjectNumber(int)));
	connect(conditionsedit,SIGNAL(textChanged(QString)),this,SLOT(setConditions(QString)));
	connect(datadirectorybutton,SIGNAL(clicked()),this,SLOT(getDataDirectory()));
	connect(conditionfilebutton,SIGNAL(clicked()),this,SLOT(getConditionFile()));	
	
	QFormLayout *layout=new QFormLayout;
	layout->addRow(tr("Subject Number"), subjectnumbox);
	layout->addRow(tr("Experiment Conditions"),conditionsedit);
	layout->addRow(tr("Preset Conditions File"),conditionfilebutton);
	layout->addRow(tr("Recorded Data Directory"), datadirectorybutton);
	layout->addRow(tr("Device to Screen Ratio"),device2screenratiobox);
	layout->addRow(tr("Screen Height (Meters)"),screenheightmetersline);
	layout->addRow(tr("Screen Width (Meters)"),screenwidthmetersline);
	layout->addRow(tr("Screen Refresh Rate (Hertz)"),refreshratebox);
	layout->addRow(quitbutton, startbutton);
	
	setLayout(layout);
}

void ParametersWidget::setSubjectNumber(int newnumber)
{
	subjectnumbox->setValue(newnumber);
	params.subjectNumber=newnumber;
	QFile qfile(conditionFile);
	qfile.open(QIODevice::ReadOnly);
	if(qfile.exists()&&(newnumber>0))
	{
		char line[101];
		int count=0;
		while(count<newnumber)
		{
			if(qfile.readLine(line, 100)<=0) break;
			count++;
		}
		QRegExp rx("(\\d+) ([\\w ]*)");
		if(rx.indexIn(line)==-1) return;
		conditionsedit->setText(rx.cap(2));
	}
	
}

void ParametersWidget::getDataDirectory() 
{
	QString temp=QFileDialog::getExistingDirectory(this, tr("Data Directory"),dataDirectory,QFileDialog::ShowDirsOnly);
	if(!temp.isEmpty()) setDataDirectory(temp);
}
void ParametersWidget::getConditionFile() 
{
	QString temp=QFileDialog::getOpenFileName(this, tr("Preset Conditions File"),dataDirectory,"*.dat");
	if(!temp.isEmpty()) setConditionFile(temp);
}
	
void ParametersWidget::setDataDirectory(QString dataDir)
{
	dataDirectory=dataDir;
	datadirectorybutton->setText(dataDir);
	QStringList filters;
	QDir dir(dataDir);
	filters << "*Cont.dat";
	dir.setNameFilters(filters);
	dir.setFilter(QDir::Files);
	QStringList files=dir.entryList();
	QRegExp rx("(\\d+)Cont.dat");
	int max=0;
	int min=1;
	foreach(QString file, files)
	{
		if(rx.indexIn(file) != -1)
		{
			int val=rx.cap(1).toInt();
			if(val> max) max=val;
			if(val< min) min=val;
		}
	}
	subjectnumbox->setMinimum(min-1);
	subjectnumbox->setMaximum(max+1);
	setSubjectNumber(max+1);
}
	
void ParametersWidget::setConditionFile(QString condfile)
{
	conditionFile=condfile;
	conditionfilebutton->setText(condfile);
}

void ParametersWidget::tryaccept()
{
	QString num;
	QFile qfile;
	QMessageBox::StandardButton reply;
	
	//If selected directory somehow does not exist, prompt to create it.
	QDir dir(dataDirectory);
	if(!dir.exists())
	{
		reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "Directory Doesn't Exist. Make it?", QMessageBox::Yes | QMessageBox::No);
		if (reply==QMessageBox::No) return;
		dir.mkpath(dataDirectory);
	}
		
	//Check that prior subject is used.
	if(!((params.subjectNumber == 1)||(params.subjectNumber == 0)))
	{
		if(params.subjectNumber>1) num.setNum(params.subjectNumber-1);
		else num.setNum(params.subjectNumber+1);
		qfile.setFileName(QString(dataDirectory+num+"Cont.dat"));
		if(!qfile.exists())
		{
			reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "Prior subject number unused!?!? Procede anyways?", QMessageBox::Yes | QMessageBox::No);
			if (reply==QMessageBox::No) return;
		}
	}
	//Check that present value is unused.
	num.setNum(params.subjectNumber);
	qfile.setFileName(dataDirectory+num+"Cont.dat");
	if(qfile.exists())
	{
		
		reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "This subject number is already used. Overwrite the old data?", QMessageBox::Yes | QMessageBox::No);
		if (reply==QMessageBox::No) return;
	}
	
	params.screenSizeMeters=point(screenwidthmetersline->value(),screenheightmetersline->value());
	params.screenRefreshRate=refreshratebox->value();
	params.device2screenRatio=device2screenratiobox->value();
	params.conditions=conditionsedit->text();
	accept();
}

