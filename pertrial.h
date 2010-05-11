#ifndef PERTRIAL_H
#define PERTRIAL_H

#define TAB << "\t" <<
#include <QTextStream>
#include <limits>
#include "point.h"
#include "devicesampler.h"

class PerTrial
{
public:
	struct ContinuousInTrial
	{
		DeviceSampler::DeviceData data; //Position and time
		Point<double> augmentedPosition;
		double normalExcursion; //Magnitude
		double normalVelocity; //Magnitude
		double directionalError;
		double dist; //Magnitude
	};
	
	struct Error {double min; double max; double average;} normalExcursion, normalVelocity, directionalError;
	double startingDistance;
	int trialLength;
	double trialTime;
	double absoluteStartTime;
	Point<double> target, origin;
	double pathLengthMouse;
	double pathLengthAugmented;
	double pathLengthMouseCorrection;
	double pathLengthAugmentedCorrection;
	double delta;
	double rotation;
	
	void reset(ContinuousInTrial &continuous, Point<double> &targetPos);
	void update(ContinuousInTrial &continuous, double excursion, double velocity, Point<double> lastpos,Point<double> lastaugpos,Point<double> target);
	void dump(ContinuousInTrial &continuous, QTextStream &trialStream, int intratrial, int trials, int block, int stim, int treat, Point<double> targetPos);
};

#endif
