#include "displaywidget.h"
#include <QtGui/qevent.h>

DisplayWidget::DisplayWidget(QRect &qr, point screenInMeters, QWidget *parent) : QGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::AlphaChannel|QGL::SampleBuffers|QGL::AccumBuffer), parent, 0, Qt::X11BypassWindowManagerHint)
//Recently switched Qt::FramelessWindowHint to unmanaged, verify working keyboard
{
	STOP=false;
	
	meterSize=screenInMeters;
	
	timer.start(8, this); //120 Hz = 8.3 ms, guarantee a paint in each refresh
	setGeometry(qr);
	setCursor(QCursor(Qt::BlankCursor)); //Hide the cursor
	//setCursor(QCursor(Qt::ArrowCursor)); //Show the cursor
	setPalette(QPalette(QColor(0, 0, 0)));
	setAutoFillBackground(false); //Try to let glClear work...
	
	// Necessary for unmanaged
	activateWindow(); //Deal with actually getting keyboard
	raise(); //Make sure it's the top window
	// Necessary for unmanaged
	
	setWindowState(Qt::WindowFullScreen);
}

void DisplayWidget::keyPressEvent(QKeyEvent * event)
{
	event->accept();
	if (event->key()==Qt::Key_Escape) STOP=true;
}


DisplayWidget::~DisplayWidget()
{
	makeCurrent();
	glDeleteLists(sphereList,1);
}

void DisplayWidget::initializeGL()
{  
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	sphereList = glGenLists (1);
	QLUquadricObj *qobj=gluNewQuadric;
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glNewList(shapeList, GL_COMPILE);
	gluSphere(qobj, 1, 100, 100); //Arbitrary defaults "grid" size: 100 x 100		
	glEndList();
	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(1);
}

void DisplayWidget::resizeGL(int w, int h)
{
	double min=w>h?meterSize.Y():meterSize.X();
	//The reachable area always includes the largest sphere that will fit in it
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-meterSize.X()/2, meterSize.X()/2, -meterSize.Y()/2, meterSize.Y()/2,-min,min);
	update();
}

void DisplayWidget::paintGL()
{
	timer.stop();
	dataMutex.lock();
	glClearColor(backgroundColor.X(), backgroundColor.Y(), backgroundColor.Z(),1);
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	for(vector<sphere>::iterator it=spheres.begin();it!=v.end();++it)
	{
		if(it->radius<=0) continue;
		glColor3dv(it->color);
		glPushMatrix();
		glTranslated(it->position.X(),it->position.Y(),it->position.Z());
		glScaled(it->radius,it->radius,it->radius);
		glCallList(sphereList);
		glPopMatrix();
	}
	dataMutex.unlock();
	
	glswap();
	glFinish();  //Get precise timing, blocks until swap succeeds.  Swap happens during refresh.
	timespec last=getTime();
	dataMutex.lock();
	lastRefresh=last;
	dataMutex.unlock();
	
	timer.start(8, this); //120 Hz = 8.3 ms, guarantee a paint in each refresh
}

