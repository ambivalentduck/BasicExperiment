#include "devicesampler.h"
#include "mousehandler.h"


DeviceSampler::DeviceSampler(timespec Start, Devices devToUse, int width, int height)
{
	w2=width/2;
	h2=height/2;
	first=First;
	STOP=false;
	START=false;
	readable.acquire(readable.available()); //0 readable
	switch(deviceToUse)
	{
	case HAPI:
		device=new HapiHandler(
	else device=mouse;
	listMutex.unlock();
}

void DeviceSampler::run()
{
	runMutex.lock();
	MouseHandler device(w2, h2); //Switch device to a union of mouse/robot classes
	DeviceData d;
	timespec last;
	while(!START)
	{
		d.p=device.acquire();
		d.t=timespecDiffToSec(s, getTime());
		listMutex.lock();
		if(readable.available()==0)
		{
			data=d;
			readable.release();
		}
		else
			data=d;
		listMutex.unlock();
		usleep(100);
	}
	while(!STOP) //tight loop, grab data as fresh as possible
	{
		d.p=device.acquire();
		d.t=timespecDiffToSec(s, getTime());
		listMutex.lock();
		if((data.p==d.p)&&((d.t-data.t)<.01)) 
		{
			listMutex.unlock();
			usleep(100);
			continue;
		}
		data=d;
		listMutex.unlock();
		readable.release();
		usleep(100);
	}
	runMutex.unlock();
}

bool DeviceSampler::tryAcquire(int timeout, DeviceData &d)
{
	if(!readable.tryAcquire(1, timeout))
	{
		return false;
	}
	listMutex.lock();
	d=data; //Copy data into d
	listMutex.unlock();
	return true;
}

