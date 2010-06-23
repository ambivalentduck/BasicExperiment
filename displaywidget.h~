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

//#define SHOWBAR
//#define REALDATA

class DisplayWidget : public QGLWidget
{
	Q_OBJECT 
	
public:
	DisplayWidget(QRect &qr, QWidget *parent=0);
	~DisplayWidget() {emit die();}
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void timerEvent(QTimerEvent * event) {update();}
	void keyPressEvent(QKeyEvent * event);
	void setReport(QString r) {dataMutex.lock(); report=r; dataMutex.unlock();}
	void setTarget(point p, point c=point(1,0,0), double r=-1) {dataMutex.lock(); target=p; targetColor=c; if(r>=0) targetRadius=r; dataMutex.unlock();}
	void setCursor(point p, point c=point(0,0,1), double r=-1) {dataMutex.lock(); cursor=p; cursorColor=c; if(r>=0) cursorRadius=r; dataMutex.unlock();}
	void setOrigin(point p, point c=point(0,0,1), double r=-1) {dataMutex.lock(); origin=p; originColor=c; if(r>=0) originRadius=r; dataMutex.unlock();}
	void setBackgroundColor(point c=point(0, 0, 0)) {dataMutex.lock(); backgroundColor=c; dataMutex.unlock();}
	timespec getLastRefresh() {return lastRefresh;}
	
private:
	void drawGLCircle(double rad=1.0, int points=20);   
	GLuint shapeList, deltaList;
	int W, H, widgetW, widgetH;
	QBasicTimer timer;
	QGLPixelBuffer * delta;
	GLuint deltaTexture;
	bool STOP;
	point target, cursor, origin, targetColor, cursorColor, originColor, backgroundColor;
	double targetRadius, cursorRadius, originRadius;
	QString report;
	timespec lastRefresh;
	QMutex dataMutex;
	
signals:
	void die();
};

#endif
