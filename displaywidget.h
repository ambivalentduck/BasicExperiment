#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QRect>
#include <QGLPixelBuffer>
#include <QString>
#include <QMutex>
#include <QBasicTimer>
#include "point.h"
#include "timestuff.h"
#include <vector> 

//#define SHOWBAR
//#define REALDATA

class DisplayWidget : public QGLWidget
{
	Q_OBJECT 
	
public:
	struct Sphere
	{
		point position;
		double radius;
		point color;
	};

	DisplayWidget(QRect &qr, point screenInMeters, QWidget *parent=0);
	~DisplayWidget();
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void timerEvent(QTimerEvent * event) {update();}
	void keyPressEvent(QKeyEvent * event);
	timespec getLastRefresh() {return lastRefresh;}
	void setBGColor(point &color) {dataMutex.lock; bgcolor=color; dataMutex.unlock;}
	void setSpheres(vector<Sphere> &s) {dataMutex.lock; spherse=s; dataMutex.unlock;}
	bool keyboardDie(return STOP;)
	
private:
	point meterSize;
	vector<Sphere> spheres;
	GLuint sphereList;
	int W, H, widgetW, widgetH;
	QBasicTimer timer;
	bool STOP;
	point backgroundColor;
	timespec lastRefresh;
	QMutex dataMutex;
};

#endif
