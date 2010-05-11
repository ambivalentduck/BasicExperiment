#include "targetcontrol.h"

TargetControl::TargetControl(double Spawndist, double Innerradius, double Maxradius, int Uses, int Directions)
{
	maxRadius=Maxradius;
	spawnDist=Spawndist;
	innerRadius=Innerradius;
	direction=new double[Directions];
	timesUsed=new int[Directions];
	directions=Directions;
	direction[0]=0;
	for(int k=1;k<directions;k++) direction[k]=direction[k-1]+2*3.14159265/directions;
	uses=Uses;
	reset(Uses);
	nextDirection=-1;
}

void TargetControl::reset(int Uses)
{
	if(Uses==-1) Uses=uses;
	for(int k=0;k<directions;k++) timesUsed[k]=0;
	minUses=Uses;
}

Point<double> TargetControl::genTarget(Point<double> currentPos, bool directionMatters)
{
	double theta;
	Point<double> targetPos;
		
	if(!directionMatters) //Direction not constrained this trial
	{	
		do 
		{
			theta=randb(0,360);
			targetPos=currentPos+Point<double>(cos(theta),sin(theta))*spawnDist;
		} 	while(targetPos.mag()>innerRadius);
		return targetPos;
	}
	
	//If direction predetermined, use it
	if (nextDirection>=0) {targetPos=currentPos+Point<double>(cos(nextDirection),sin(nextDirection))*spawnDist; nextDirection=-1; return targetPos;} 
	
	//Make sure we haven't bottomed out
	int sum=0;
	for(int k=0;k<directions;k++) 
	{
		sum+=((timesUsed[k]>=minUses)?0:1);
	}
	if (sum==0) minUses++;
	
	//Find the best direction
	double min=std::numeric_limits<double>::infinity();
	int index=-1;
	double mag;
	Point<double> bestPos;
	for(int k=0;k<directions;k++)
	{
		targetPos=currentPos+Point<double>(cos(direction[k]),sin(direction[k]))*spawnDist;
		mag=targetPos.mag();
		if((mag<min)&&(timesUsed[k]<minUses)&&(mag<innerRadius)) {min=mag; index=k; bestPos=targetPos;}
	}
	if(index != -1)
	{
		timesUsed[index]++;
		return bestPos;
	}
	
	//If no direction can produce a legal target pick randomly among the least used and set up for it
	int m=INT_MAX;
	int i;
	int * shuffle = new int[directions];
	shuffle[0]=directions;
	randperm(shuffle);
	for(int k=0;k<directions;k++)
		if(timesUsed[shuffle[k]]<m) {m=timesUsed[shuffle[k]]; i=shuffle[k];}
	timesUsed[i]++;
	nextDirection=direction[i];
	theta=atan2(currentPos.Y()+spawnDist*sin(direction[i]),currentPos.X()+spawnDist*cos(direction[i]));
	targetPos=currentPos - Point<double>(cos(theta),sin(theta))*spawnDist;
	return (targetPos.mag()<=maxRadius)?targetPos:Point<double>(0,0); 
}

