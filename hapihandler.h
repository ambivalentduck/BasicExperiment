#ifndef HAPIHANDLER_H
#define HAPIHANDLER_H

#include "point.h"
#include "timestuff.h"
#include "forcepulse.h"

class HAPIHandler: public DeviceHandler
{
public:
	HAPIHandler(point Center, double Radius);
	deviceState acquire();
	bool setForce(point force, double duration=-1);
	bool rateIsVariable() {return true;}
	double getMaxRate() {return device->getHapticsRate();}
	
private:
	HAPI::auto_ptr<AnyHapticsDevice> device;
	const point min=point(-.05, -.05, -0.75), max=point(.05, .05, .025);
	const point ave=(max-min)/2l;
	double start, sunset;
};

HAPIHandler::HAPIHandler(point Center, double Radius): DeviceHandler(point Center, double Radius)
{
	device=new HAPI::AnyHapticsDevice;
	if(device->initDevice() != HAPI::HAPIHapticsDevice::SUCCESS) {cerr << device->getLastErrorMsg();}
	device->clearEffects();
	device->transferObjects();
	device->enableDevice();
	device->clearEffects();
	device->transferObjects();
	start=getTime();
}

deviceState HAPIHandler::acquire()
{
	if((start-getTime())>sunset)
	{
		device->clearEffects();
		device->transferObjects();
	}
	double rate=device->getHapticsRate();
	point current;
	current.position=device->getPosition();
	current.velocity=device->getVelocity();
	current.appliedforce=(current.velocity-device->getLastVelocity())/device->getHapticsRate();
	return current;	
}

bool HAPIHandler::setForce(point force, double duration)
{
	sunset=duration+(start-getTime());
	device->addEffect(new ForcePulse(force,duration));
	device->transferObjects();
	return true;
}


#endif
