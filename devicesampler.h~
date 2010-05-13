#ifndef DEVICESAMPLER_H
#define DEVICESAMPLER_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <time.h>
#include <string.h>
#include "point.h"
#include "timestuff.h"
#include <QList>

#define DEVICE_DATA_SIZE 120

class DeviceSampler : public QThread
{
public:
	enum Devices {MOUSE, HAPI, XPC};
	DeviceSampler(timespec First, Devices devToUse, int width, int height);
	struct DeviceData {point p; point v; point f; double t;};
	void run();    
	void die() {STOP=true; runMutex.lock();}
	bool tryAcquire(int timeout, DeviceData &d);
	
private:
	timespec first;
	bool START, STOP;
	QMutex runMutex, listMutex;
	int w2,h2;
	QSemaphore readable;
	DeviceData data;
	DeviceHandler * device;
};

#endif

