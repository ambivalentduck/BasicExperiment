#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QThread>
#include <QMutex>
#include <cstring>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "timestuff.h"
#include "devicesampler.h"
#include "pertrial.h"
#include "targetcontrol.h"

#define RAD 8
#define BLOCK_LENGTH 50

class DeepThought : public QThread
{
public:
	DeepThought(QRect QR, QString Task, QString Treatment, QWidget *parent=0);
	void run();
	void die() {STOP=true; runMutex.lock(); sampler->die(); msleep(10);} 
	
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
	point phi, jointPos, endPos;
};

#endif
