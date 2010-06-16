#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QThread>
#include <QMutex>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "timestuff.h"
#include "devicesampler.h"
#include "pertrial.h"
#include "targetcontrol.h"
#include "displaywidget.h"

#define RAD 8 //Pixel size of cursors/targets on screen
#define BLOCK_LENGTH 50
#define SCREEN_REFRESH 60 //60 Hz

class Experiment: public QThread
{
public:
	Experiment(QRect QR, QString Task, QString Treatment, QWidget *parent=0);
	void run();
	void die() {STOP=true; runMutex.lock(); sampler->die(); msleep(10);}
	~Experiment() {die();}
	
	PerTrial::ContinuousInTrial continuous;
	PerTrial pertrial;
	
	DisplayWidget * widget;
        
private:
	void makeTrial(bool success=true);
	void noConsecutive(bool * array, int n);
	
	timespec s;
	DeviceSampler * sampler;
	bool STOP, START;
	QMutex runMutex;
	char report[40];
	point cursorPos, targetPos, lastTargetPos, targetColor, lastpos, lastaugpos, origin;
	
	int w, h;
	double min;
	
	int block, intrablock,intratrial, trials, state;
	int * taskOrder, * treatOrder;
	int treat, task;
	TargetControl target;
	
	bool killswitch, hold, freshEndError;
	double rotation, deltaP, deltaV, deviceGain, lastEndError;
	
	QFile contFile, trialFile;
	QTextStream contStream, trialStream;
	
	double lastTime, trialStartTime, holdStartTime;
	double inTargetCounter;
};

#endif
