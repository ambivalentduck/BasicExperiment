#include "displaywidget.h"
#include <QPainter>
#include <QtGui/qevent.h>
#include "pi.h"

#define TEXTX 128
#define TEXTY 12

DisplayWidget::DisplayWidget(QRect &qr, QWidget *parent) : QGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::AlphaChannel|QGL::SampleBuffers|QGL::AccumBuffer), parent, 0, Qt::X11BypassWindowManagerHint)
//Recently switched Qt::FramelessWindowHint to unmanaged, verify working keyboard
{
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
	
	#ifndef REALDATA
	delta = new QGLPixelBuffer(TEXTX, TEXTY, format(), this);
	#endif	
	
	setWindowState(Qt::WindowFullScreen);
}

void DisplayWidget::keyPressEvent(QKeyEvent * event)
{
	event->accept();
	if (event->key()==Qt::Key_Escape) emit die();
}


DisplayWidget::~DisplayWidget()
{
	makeCurrent();
	glDeleteLists(shapeList,1);
}

void DisplayWidget::initializeGL()
{  
	#ifndef REALDATA
	deltaTexture = delta->generateDynamicTexture();
	delta->makeCurrent();
	glClearColor(0,0,0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	delta->updateDynamicTexture(deltaTexture);
	#endif
		
	makeCurrent();
	glClearColor(0,0,0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	shapeList = glGenLists (1);
	glNewList(shapeList, GL_COMPILE);
	drawGLCircle(RAD,80);
	glEndList();
	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(1);
}

void DisplayWidget::drawGLCircle(double rad, int points)
{
	double i;
	glBegin(GL_POLYGON);
	for(int k=0; k<=points;k++)
	{
		i=2*PI*double(k)/double(points);
		glVertex3d(rad*cos(i),rad*sin(i),0);
	}
	glEnd();
}

void DisplayWidget::resizeGL(int w, int h)
{
	widgetW=w;
	widgetH=h;
	W=w;
	H=h;
	glViewport(0,0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-W/2, W/2, -H/2, H/2,-W,W);
	
	update();
}

void DisplayWidget::paintGL()
{
	timer.stop();
	#ifndef REALDATA 
	delta->makeCurrent();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	QPainter paint(delta);
	paint.setBrush(Qt::white);
	paint.setPen(Qt::white);
	//paint.setFont(QFont("Times",12));
	dataMutex.lock();
	paint.drawText(8,TEXTY-2,report);
	dataMutex.unlock();
	paint.end();
	glFlush();
	delta->updateDynamicTexture(deltaTexture);
	#endif
	
	dataMutex.lock();
	makeCurrent();
	
	glClearColor(backgroundColor.X(), backgroundColor.Y(), backgroundColor.Z(),0);
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
	
	#ifndef REALDATA
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, deltaTexture);
	
	glBegin(GL_POLYGON);
	glTexCoord2d(0,1); glVertex2d(-W/2,H/2); //Top left
	glTexCoord2d(0,0); glVertex2d(-W/2,H/2-TEXTY); //Bottom Left
	glTexCoord2d(1,0); glVertex2d(-W/2+TEXTX,H/2-TEXTY); //Bottom Right
	glTexCoord2d(1,1); glVertex2d(-W/2+TEXTX,H/2); //Top Right
	glEnd();
	glDisable(GL_TEXTURE_2D);
	#endif
	
	if(targetRadius>0)
	{
		glColor3dv(targetColor); //Draw Target
		glPushMatrix();
		glTranslated(target.X(),target.Y(),target.Z());
		if(targetRadius>1)
		{
			glScaled(targetRadius,targetRadius,targetRadius);
			glCallList(shapeList);
		}
		else
		{
			glBegin(GL_POINTS);
				glVertex2d(0,0);
			glEnd();
		}
		glPopMatrix();
	}
	
	if(originRadius>0)
	{
		glColor3dv(originColor); //Draw Origin
		glPushMatrix();
		glTranslated(origin.X(),origin.Y(),origin.Z());
		if(originRadius>1)
		{
			glScaled(originRadius,originRadius,originRadius);
			glCallList(shapeList);
		}
		else
		{
			glBegin(GL_POINTS);
				glVertex2d(0,0);
			glEnd();
		}
		glPopMatrix();
	}
	
	if(cursorRadius>0)
	{
		glColor3dv(cursorColor); //Draw Cursor
		glPushMatrix();
		glTranslated(cursor.X(),cursor.Y(),cursor.Z());
		if(cursorRadius>1)
		{
			glScaled(originRadius,originRadius,originRadius);
			glCallList(shapeList);
		}
		else
		{
			glBegin(GL_POINTS);
				glVertex2d(0,0);
			glEnd();
		}
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

