#include "point.h"

class DeviceHandler
{
public:
	struct deviceState
	{
		point position;
		point velocity;
		point appliedforce;
	};
		
	DeviceHandler(point Center, double Radius) {rate=-1, center=Center; radius=Radius}; 
	//Derived classes constructors should take a point input that defines the desired center and double that denotes a screen radius.

	virtual deviceState acquire();
	
	virtual bool setForce(point force, double duration=-1) {return false;}
	
	virtual bool rateIsVariable() {return true;}
	virtual double getMaxRate() {return rate;}
	virtual bool setRecordingRate(double desired) {return false;}
	
	double rate, radius;
	point center;
};
