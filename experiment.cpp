#include "experiment.h"
#include <stdlib.h>
#include <math.h>
#include <limits>
#include <iostream>
#include "randb.h"


#define TAB << "\t" <<
#define CURLMAG 5.0
#define ROTAMOUNT -45
#define TRIAL_DURATION .5
#define HOLD_TIME .75
#define TARGET_HOLD_TIME .5

//         		 Washout   Train    Test
const int BlockLength[]={30,       50,      30};
//const int BlockLength[]={3,       5,      3};

Experiment::Experiment(QRect QR, QWidget *parent)
{
	init_randb();
	
	//Most pressing task is to get a subject#, refresh rate, screen size in meters.
	
	min=(QR.width()>QR.height())?QR.height():QR.width();
	//Lesser screen dimension, since targets are generated within a set radius, circle diameter
	
	target=new targetControl(.9*min/3l, min/3l, min/2l);
	//Targets are .3 screens apart, requested to be in the middle 2/3 of the screen, and the entire lesser dimension is legal 
		
	
	killswitch=false;
	w=W;
	h=H;
	s=first;
	sampler=Sampler;
	srand(time(NULL));
	cursorPos=point(0,0);
	targetPos=point(0,0);
	lastTargetPos=targetPos;
	targetColor=point(1,0);
	rotation=0.0;
	intratrial=-1;
	trials=-1;
	deviceGain=1;
	lastEndError=0;
	
	contFile.setFileName(output+"Cont.dat"); contFile.open(QIODevice::WriteOnly | QIODevice::Text); contStream.setDevice(&contFile);
	trialFile.setFileName(output+"Trial.dat"); trialFile.open(QIODevice::WriteOnly | QIODevice::Text); trialStream.setDevice(&trialFile);
	
	state=-1;
	
	report="";
	
	block=0;
	intrablock=-1;
	inTargetCounter=0;
	
	makeTrial();
}

void Experiment::run()
{
	runMutex.lock();
	setPriority(QThread::TimeCriticalPriority);
	point optimal, errorP, errorV, dP, zero(0,0,0),rot,X(1,0),Y(0,1),Z(0,0,1), cumV(0,0), armpos;
	double dPmag;

	while(!(STOP||widget.keyboardDie()))
	{
		if(!sampler->tryAcquire(continuous.data, 16)) continue; //Check every 16 ms for a reason to die even if no data.
				
		//Finite state machine goes here.
		
		
		if(continuous.augmentedPosition.dist(zero)>(min/2)) continuous.augmentedPosition=continuous.augmentedPosition.unit()*min/2; //Prevent off-screen cursor
	
		//Most fsm's require some sort of movement detection and time based sunset.  Move this code to a function.
		if (!hold) 
		{
			if (stim==CURL) cursorPos=continuous.augmentedPosition+point(-CURLMAG*cumV.Y(),CURLMAG*cumV.X());
			else cursorPos=continuous.augmentedPosition;
		}
		
		pertrial.update(continuous,continuous.data.p.linedist(origin,targetPos), errorV.mag()/(continuous.data.t-lastTime), lastpos, lastaugpos, targetPos); //Handle finding new min/max/ave values
		
		sprintf(report, "t%c s%c %i", treats[treat+1],stims[stim+1],intrablock);
		
		contStream << trials TAB continuous.data.t TAB continuous.data.p TAB continuous.drawnPosition TAB hold << std::endl;
		contStream.flush();
		intratrial++;
		if(killswitch==true) {sprintf(report, "killMe"); break;}
		if((cursorPos.dist(targetPos)<(2*RAD))&(!hold))  //Make > to instantly do whole experiment.		
		{
			targetColor=point(0,1,0);
			inTargetCounter+=continuous.data.t-lastTime;
			if(inTargetCounter>=TARGET_HOLD_TIME) makeTrial();
		}
		else
		{
			if((cursorPos.dist(lastTargetPos)>(RAD))&(trialStartTime>9000)) trialStartTime=continuous.data.t;
			if(!hold)
			{
				inTargetCounter=0;
				targetColor=point(1,0,0);
				if ((continuous.data.t-trialStartTime)>=TRIAL_DURATION) {hold=true; holdStartTime=continuous.data.t;} //Hold after a certain time
			}
			else 
			{
				if (dPmag>0) holdStartTime=continuous.data.t;
				if((continuous.data.t-holdStartTime)>=HOLD_TIME) makeTrial();
			}
		}
		lastpos=continuous.data.p;
		lastaugpos=continuous.augmentedPosition;
		lastTime=continuous.data.t;
	}
	runMutex.unlock();
}

void Experiment::makeTrial(bool success)
{
	if (trials!=-1) //Finish calculations and write to file
	{
		pertrial.dump(continuous, trialStream, intratrial, trials, block, stim, treat, targetPos);
	}
	
	targetColor=point(1,0,0);
	trialStartTime=9900;
	hold=false;
	double dist=(targetPos-cursorPos).mag()-2*RAD;
	lastEndError=(dist>0?dist:0)/(.6*min/3);
	freshEndError=true;
	lastTargetPos=continuous.augmentedPosition;
	
	trials++;
	
	if(trials==0)
	{
		sampler->tryAcquire(-1,continuous.data);
		lastTime=continuous.data.t;
		lastpos=continuous.data.p;
		lastaugpos=continuous.data.p;
		cursorPos=continuous.data.p;
	}
	
	continuous.normalExcursion=0;
	continuous.normalVelocity=0;
	continuous.dist=lastaugpos.dist(targetPos);
	origin=lastaugpos;
	
	targetPos=target.genTarget(continuous.augmentedPosition);
	if(intrablock>=(BlockLength[block%3]-1)) {block++; target.reset(); intrablock=-1;}
	intrablock++;
	if(block<=9) switch(block%3)
	{
	case 0: //   Intro/Washout
		state=-1;
		stim=-1;
		treat=-1;
		break;
		
	case 1: //   Train Enhanced
		state=(block-1)/3;
		stim=stimOrder[state];
		treat=treatOrder[state];
		break;
		
	case 2: //   Test unenhanced
		state=(block-2)/3;
		stim=stimOrder[state];
		treat=-1;
		break;       
	}
	else
	{
		targetPos=target.genTarget(continuous.augmentedPosition);
		killswitch=true;
		std::cout << "Done! :)" << std::endl;
	}
	
	if (stim==ROTATION) rotation=ROTAMOUNT;
	else rotation=0;
	if ((treat==EA)||(treat==X2pEA)) deltaP=2;
	else deltaP=1;
	if ((treat==VEA)||(treat==X2pVEA)) deltaV=2;
	else deltaV=1;
	if (treat==X4) deviceGain=4;
	else if ((treat==SENSITIVITY)||(treat==X2pVEA)||(treat==X2pEA)) deviceGain=2;
	else deviceGain=1;
	if (stim==FNRAO)
	{
		point diff=targetPos-origin;
		double phi1=atan2(diff.Y(),diff.X());
		phi=point(phi1-PI/2,PI);
	}
	pertrial.rotation=rotation;
	pertrial.delta=deltaP;
	intratrial=1;
	pertrial.reset(continuous, targetPos); 
}

