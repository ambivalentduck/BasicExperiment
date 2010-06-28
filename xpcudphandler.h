#ifndef XPCUPDHANDLER_H
#define XPCUDPHANDLER_H

#include <QtNetwork>
#include <QByteArray>
#include "point.h"

class DeviceHandler
{
public:
	XpcUdpHandler(point Center): DeviceHandler(Center)
	{
		us.bind(7755); //Ie Whatever xpc uses

	deviceState acquire()
	{
		state.position=point();
		state.velocity=point();
		state.appliedforce=point();
		return state;
	}
	
	bool setForce(point force, double start, double duration=-1) {return false;}
	
	virtual bool rateIsVariable() {return false;}
	virtual double getMaxRate() {return 1000;} //obv fix this
	
	double rate, radius;
	point center;
	timespec start;
	QUdpSocket us;
	QByteArray out, in;
};

#endif

/*
QUdpSocket us;
us.bind(7755);
QByteArray out, in;
out.resize(3+sizeof(double));
double test=75;	
char * cd=reinterpret_cast<char*>(&test);
out=QByteArray(cd,sizeof(double));
out.prepend("abc");
out.append("q");
std::cout<<test<<" "<<out.data()<<std::endl;
us.writeDatagram(out, QHostAddress::LocalHost, 7755);
while(!us.hasPendingDatagrams());
in.resize(us.pendingDatagramSize());
us.readDatagram(in.data(), in.size());
std::cout<<out.size()<<" "<<in.size()<<std::endl;
for(int k=0;k<5;k++)
{
	double * testresult=reinterpret_cast<double*>(in.data()+k);
	std::cout<<in.data()<<" "<<*testresult<<std::endl;
}
return 0;
*/
