#include <X11/Xlib.h>
#include "point.h"
#include <time.h>
#include "timestuff.h"
#include "devicehandler.h"

class MouseHandler: public DeviceHandler
{
public:
	MouseHandler(point Center, double Radius)
	{
		d = XOpenDisplay(NULL);
		w2=W2;
		h2=H2;
		home = Point<double>(w2,h2);
		last = home;
		XWarpPointer(d,RootWindow(d,DefaultScreen(d)),RootWindow(d,DefaultScreen(d)),0,0,0,0,home.X(),home.Y());
		offset=Point<double>(home.X()-w2+0.5,h2-home.Y()-0.5);
		clock_gettime(CLOCK_REALTIME,&t1);
		t2=t1;
		rate=120;
		baseT=1.0/120.0;
		deviceState state;
		state.appliedforce=point();
	}
	
	Point<double> acquire() 
	{
		XQueryPointer(d,RootWindow(d,DefaultScreen(d)),&e.xbutton.root,&e.xbutton.window,&e.xbutton.x_root,&e.xbutton.y_root, &e.xbutton.x,&e.xbutton.y,&e.xbutton.state); 
		current = Point<double>(e.xbutton.x_root-last.X(),last.Y()-e.xbutton.y_root);
		last=Point<double>(e.xbutton.x_root,e.xbutton.y_root);
		offset+=current;
		clock_gettime(CLOCK_REALTIME,&t2);
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
};