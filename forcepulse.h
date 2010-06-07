#ifndef FORCEPULSE_H
#define FORCEPULSE_H

#include <HAPI/HAPIForceEffect.h>

class forcepulse: public HAPI::HAPIForceEffect
{
public:
	forcepulse(HAPI::Vec3 Force, double Time) {force=Force, time=Time, used=false;}
	HAPI::HAPIForceEffect::EffectOutput calculateForces(const HAPI::HAPIForceEffect::EffectInput &input)
	{
		if(used==false)
		{
			firstused=HAPI::TimeStamp();
			used=true;
		}
		if(((double(HAPI::TimeStamp())-double(firstused))<time)||(time<0))
		{
			return HAPI::HAPIForceEffect::EffectOutput(force);
		}
		return HAPI::HAPIForceEffect::EffectOutput();
	}
	HAPI::Vec3 force;
	double time, firstused;
	bool used;
};

#endif
