#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QThread>
#include <QMutex>
#include <cstring>
#include <QString>
#include <time.h>
#include "timestuff.h"
#include "devicesampler.h"
#include <QFile>
#include <QTextStream>
#include "pertrial.h"
#include "targetcontrol.h"

#define RAD 8
#define BLOCK_LENGTH 50-1 //-1 to fix length



class DeepThought : public QThread
{
public:
	DeepThought(int enhance, int task);
	void run();
	void die() {STOP=true; runMutex.lock(); sampler->die(); msleep(10);} 
	QString getReport() {return QString(report);}
	point getTargetDest() {return targetPos;}
	point getTargetOrigin() {return lastTargetPos;}
	point getCursor() {return cursorPos;}
	point getTargetColor() {return targetColor;}
	point getScreenColor() {if (hold) return point(0,0,1); else return point(0,0,0);}
	point getJoint() {return jointPos;}
	point getOrigin() {return origin;}
	point getEnd() {return endPos;}
	double getRotation() {return 0.0;} //rotation;}
	bool getHold() {return !hold;}
	bool getEndError(double &error) {error=lastEndError; if (freshEndError) {freshEndError=false; return true;} else return false;}
	
	PerTrial::ContinuousInTrial continuous;
	PerTrial pertrial;
	
	enum //Enumerate tasks and enhancements here
	{
		CURL=0,
		ROTATION=1,
		REFLECT=2,
		FNRAO=3,
		LIN=4,
		CONTROL=0,
		EA=1,
		SENSITIVITY=2,
		VEA=3,
		X2pEA=4,
		X2pVEA=5,
		X4=6,
		UNCERTAIN=7
	};
        
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
	int * stimOrder, * treatOrder;
	int stimorder[4], treatorder[4];
	int treat, stim;
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
