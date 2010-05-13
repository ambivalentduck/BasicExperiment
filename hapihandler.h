#ifndef HAPIHANDLER_H
#define HAPIHANDLER_H

#include "point.h"
#include "timestuff.h"

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
	const point min, max=point(-.05, -.05, -0.75), max=point(.05, .05, .025);
};

#endif


HAPIHandler::HAPIHandler(point Center, double Radius): DeviceHandler(point Center, double Radius)
{
	device=new HAPI::AnyHapticsDevice;
	if(device->initDevice() != HAPI::HAPIHapticsDevice::SUCCESS) {cerr << device->getLastErrorMsg();}
	device->clearEffects();
	device->enableDevice();
}

