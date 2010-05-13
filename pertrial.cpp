#include "pertrial.h"

void PerTrial::reset(ContinuousInTrial &continuous, Point<double> &targetPos)
{
	normalVelocity.max=0.0;
	normalVelocity.min=std::numeric_limits<double>::infinity();
	normalVelocity.average=0.0;
	normalExcursion.max=0.0;
	normalExcursion.min=std::numeric_limits<double>::infinity();
	normalExcursion.average=0.0;
	directionalError.max=0.0;
	directionalError.min=1;
	directionalError.average=0.0;
	
	startingDistance=continuous.augmentedPosition.dist(targetPos);
	absoluteStartTime=continuous.data.t;
	origin=continuous.augmentedPosition;
	target=targetPos;
	pathLengthMouse=0.0;
	pathLengthAugmented=0.0;
}
	
void PerTrial::update(ContinuousInTrial &continuous, double excursion, double velocity, Point<double> lastpos,Point<double> lastaugpos,Point<double> target)
{
	//Directional Error
	if (continuous.directionalError>directionalError.max) directionalError.max=continuous.directionalError;
	if (continuous.directionalError<directionalError.min) directionalError.min=continuous.directionalError;
	directionalError.average+=continuous.directionalError;
	
	//Normal Excursion
	if (excursion>normalExcursion.max) normalExcursion.max=excursion;
	if (excursion<normalExcursion.min) normalExcursion.min=excursion;
	normalExcursion.average+=excursion;
	
	//Normal Velocity
	if (velocity>normalVelocity.max) normalVelocity.max=velocity;
	if (velocity<normalVelocity.min) normalVelocity.min=velocity;
	normalVelocity.average+=velocity;
	
	pathLengthMouse+=continuous.data.p.dist(lastpos);
	pathLengthAugmented+=continuous.augmentedPosition.dist(lastaugpos);
	continuous.dist=continuous.augmentedPosition.dist(target);
}

void PerTrial::dump(ContinuousInTrial &continuous, QTextStream &trialStream, int intratrial, int trials, int block, int stim, int treat, Point<double> targetPos)
{
	normalVelocity.average/=intratrial;
	normalExcursion.average/=intratrial;
	directionalError.average/=intratrial;
	trialTime=continuous.data.t-absoluteStartTime;
	pathLengthMouseCorrection=pathLengthMouse/startingDistance;
	pathLengthAugmentedCorrection=pathLengthAugmented/startingDistance;
	trialStream <<block TAB trials TAB 
		      stim TAB
		      treat TAB
		      intratrial TAB
		      trialTime TAB
		      targetPos.X() TAB targetPos.Y() TAB
		      origin.X() TAB origin.Y() TAB
		      normalVelocity.average TAB
		      normalVelocity.max TAB
		      normalVelocity.min TAB
		      normalExcursion.average TAB
		      normalExcursion.max TAB
		      normalExcursion.min TAB
		      directionalError.average TAB
		      directionalError.max TAB
		      directionalError.min << "\n";
	trialStream.flush();
}

