#ifndef TARGETCONTROL_H
#define TARGETCONTROL_H

#include "point.h"
#include <math.h>
#include "randb.h"
#include <limits>
#include <limits.h>

class TargetControl
{
public:
	TargetControl(double spawndist, double innerradius, double Maxradius, int Uses=5, int Directions=3);
	void reset(int Uses=-1);
	Point<double> genTarget(Point<double> currentPos, bool directionMatters=false);
	
private:
	double * direction;
	double spawnDist, innerRadius;
	int uses, directions, minUses;
	int * timesUsed;
	double nextDirection;
	double maxRadius;
};

#endif

