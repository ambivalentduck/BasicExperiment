#include "deepthought.h"
#include <stdlib.h>
#include <math.h>
#include <limits>
#include <iostream>
#include "randb.h"

#define PI 3.14159265
#define TAB << "\t" <<
#define CURLMAG 5.0
#define ROTAMOUNT -45
#define TRIAL_DURATION .5
#define HOLD_TIME .75
#define TARGET_HOLD_TIME .5
#define L1 .5*(min/2)
#define L2 .5*(min/2)
#define REFLECT_ANGLE 90

//         		 Washout   Train    Test
const int BlockLength[]={30,       50,      30};
//const int BlockLength[]={3,       5,      3};

DeepThought::DeepThought(timespec first, DeviceSampler * Sampler, int W, int H, QString output, QString input, int enhance, int task) : min((W>H)?H:W), target(.6*min/3, min/3, min/2)
{
	init_randb();
	stimOrder=stimorder;
	if(task<0)
	{
		stimOrder[1]=3;
		stimOrder[0]=-1;
		stimOrder++;
		randperm(stimOrder);
	}
	else
	{
		stimOrder[0]=-1;
		stimOrder[3]=task%10;
		task-=task%10;
		task/=10;
		stimOrder[2]=task%10;
		task-=task%10;
		stimOrder[1]=task/10;
		stimOrder++;
	}
	treatOrder=treatorder;
	if(enhance<0)
	{
		treatOrder[1]=3;
		treatOrder[0]=-1;
		treatOrder++;
		randperm(treatOrder);
	}
	else
	{
		treatOrder[0]=-1;
		treatOrder[3]=enhance%10;
		enhance-=enhance%10;
		enhance/=10;
		treatOrder[2]=enhance%10;
		enhance-=enhance%10;
		treatOrder[1]=enhance/10;
		treatOrder++;
	}
	treat=-1;
	stim=-1;
	
	killswitch=false;
	hold=false;
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
	deltaP=1;
	deltaV=1;
	deviceGain=1;
	lastEndError=0;
	
	contFile.setFileName(output+"Cont.dat"); contFile.open(QIODevice::WriteOnly | QIODevice::Text); contStream.setDevice(&contFile);
	trialFile.setFileName(output+"Trial.dat"); trialFile.open(QIODevice::WriteOnly | QIODevice::Text); trialStream.setDevice(&trialFile);
	
	state=-1;
	char treats[]="W1E2VAv4";
	
	char stims[]="WCRLF";
	sprintf(report, "t%c s%c %i", treats[treat+1],stims[stim+1],0);
	
	block=0;
	intrablock=-1;
	inTargetCounter=0;
	
	makeTrial();
}

void DeepThought::run()
{
	runMutex.lock();
	setPriority(QThread::TimeCriticalPriority);
	point optimal, errorP, errorV, dP, zero(0,0,0),rot,X(1,0),Y(0,1),Z(0,0,1), cumV(0,0), armpos;
	double dPmag;
	char treats[]="W1E2VAv4";
	char stims[]="WCRLF";
	while(!STOP)
	{
		if(!sampler->tryAcquire(8,continuous.data)) continue;
		dP=(continuous.data.p-lastpos);
		dP*=deviceGain/2;
		if (stim==LIN)
		{
			dP=point(dP.X()+dP.Y(),dP.X()-dP.Y()).rotate(rotation,Z)/sqrt(2L);
		}
		else if (stim==FNRAO)
		{
			dP/=(min/4);
			phi+=dP;
			armpos=point(L1*cos(phi.X())+L2*cos(phi.X()+phi.Y()), L1*sin(phi.X())+L2*sin(phi.X()+phi.Y()))+origin;
			dP=armpos-lastaugpos;
			jointPos=point(L1*cos(phi.X()),L1*sin(phi.X()))+origin;
			endPos=armpos;
		}
		else if (stim==ROTATION) dP=dP.rotate(rotation,Z);  //Rotation is usually set to 0
		else if (stim==REFLECT) dP=dP.reflect(REFLECT_ANGLE);
		
		if ((continuous.data.t-trialStartTime)>=.250) {deltaV=1; deltaP=1; deviceGain=1;}
		dPmag=dP.mag();
		optimal=targetPos-lastaugpos;
		if((lastaugpos+dP).dist(targetPos)<lastaugpos.dist(targetPos))
		{ //Distance decreased
			continuous.directionalError=asin(fabs((optimal.unit()*dP.unit()).mag()))/(2*PI);
			errorV = dP.linepointvec(zero,optimal);
		}
		else
		{ //Distance increased
			continuous.directionalError=1.0-acos(fabs(optimal.unit()|dP.unit()))/(2*PI);
			errorV=dP;
		}
		errorP=dP.linepointvec(0,targetPos-pertrial.origin);
		
		//Augment Velocity and Position
		continuous.augmentedPosition=lastaugpos+dP+errorV*(deltaV-1.0)+errorP*(deltaP-1.0);
		if (stim==CURL) //Device V
		{
			cumV*=.5;
			cumV+=dP*.5;
		}
		/*if (stim==CURL) //Screen V
		{
		cumV*=.5;
		cumV+=(continuous.augmentedPosition-lastaugpos)*.5;
		}*/
		
		
		if(continuous.augmentedPosition.dist(zero)>(min/2)) continuous.augmentedPosition=continuous.augmentedPosition.unit()*min/2; //Prevent off-screen cursor
		
		if (!hold) 
		{
			if (stim==CURL) cursorPos=continuous.augmentedPosition+point(-CURLMAG*cumV.Y(),CURLMAG*cumV.X());
			else cursorPos=continuous.augmentedPosition;
		}
		
		pertrial.update(continuous,continuous.data.p.linedist(origin,targetPos), errorV.mag()/(continuous.data.t-lastTime), lastpos, lastaugpos, targetPos); //Handle finding new min/max/ave values
		
		sprintf(report, "t%c s%c %i", treats[treat+1],stims[stim+1],intrablock);
		
		contStream << trials TAB 
		continuous.data.t TAB
		continuous.data.p.X() TAB continuous.data.p.Y() TAB continuous.data.p.Z() TAB
		continuous.augmentedPosition.X() TAB continuous.augmentedPosition.Y() TAB continuous.augmentedPosition.Z() TAB
		continuous.directionalError TAB continuous.normalExcursion TAB continuous.normalVelocity TAB cursorPos.X() TAB cursorPos.Y() TAB hold << "\n";
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

void DeepThought::makeTrial(bool success)
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

