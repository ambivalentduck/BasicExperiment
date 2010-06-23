#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <X11/Xlib.h>
#include <time.h>
#include <QRect>
#include "point.h"
#include "timestuff.h"
#include "devicehandler.h"

//http://xorg.freedesktop.org/wiki/Development/Documentation/PointerAcceleration#Introduction
//xset m 1 100000 fixes acceleration
//xset m 2/1 4 appears to be Ubuntu's default

class MouseHandler: public DeviceHandler
{
public:
	MouseHandler(point Center, double Radius, QRect qr): DeviceHander(Center, Radius) 
	{
		d = XOpenDisplay(NULL);
		last = center;
		home=point(double(qr.width())/2l-.05, double(qr.height())/2l-.05);
		XWarpPointer(d,RootWindow(d,DefaultScreen(d)),RootWindow(d,DefaultScreen(d)),0,0,0,0,home.X(),home.Y());
		offset=point();
		t1=getTime();
		t2=t1;
		rate=120;
		baseT=1l/rate;
		state.appliedforce=point();
	}
	
	Point<double> acquire() 
	{
		XQueryPointer(d,RootWindow(d,DefaultScreen(d)),&e.xbutton.root,&e.xbutton.window,&e.xbutton.x_root,&e.xbutton.y_root, &e.xbutton.x,&e.xbutton.y,&e.xbutton.state); 
		current = point(e.xbutton.x_root-last.X(),last.Y()-e.xbutton.y_root);
		last=point(e.xbutton.x_root,e.xbutton.y_root);
		offset+=current;
		t2=getTime();
		if(timespecDiffToSec(t1, t2)>0.05) //Works out to 6 samples?
		{
			t1=t2;
			XWarpPointer(d,RootWindow(d,DefaultScreen(d)),RootWindow(d,DefaultScreen(d)),0,0,0,0,home.X(),home.Y());
			last=home;
		}
		state.position=offset;
		state.velocity=current/baseT;
		return state;
	}
	
	~MouseHandler() {XCloseDisplay(d);}
	
private:
	XEvent e;
	Display *d;
	int w2, h2;
	Point<double> offset, last, home, current;
	timespec t1, t2;
	deviceState state;
};

#endif
