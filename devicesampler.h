#ifndef DEVICESAMPLER_H
#define DEVICESAMPLER_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include "point.h"
#include "timestuff.h"
#include "devicehandler.h"

#define DEVICE_DATA_SIZE 120

class DeviceSampler : public QThread
{
public:
	enum Devices {MOUSE, HAPI, XPC_UDP};
	DeviceSampler(timespec First, Devices devToUse, point Pixels, point Meters, double recordRate, double dataRate);
	struct DeviceData {point p; point v; point f; double t;};
	void run();    
	void die() {STOP=true; runMutex.lock();}
	bool tryAcquire(DeviceData &d, int timeout=0);
	bool applyForce(point f);
	void setRecordRate(double hertz);
	void setDataRate(double hertz);
	DeviceHandler * device;
	
private:
	timespec zero;
	bool START, STOP;
	QMutex runMutex, listMutex;
	point pixels, meters;
	QSemaphore readable;
	vector<DeviceData> data;
};

#endif

