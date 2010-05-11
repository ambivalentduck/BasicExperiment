#include "displaywidget.h"
#include <QPainter>
#include <QtGui/qevent.h>

#define TEXTX 128
#define TEXTY 12
#define BAR_WIDTH 40
#define REALDATA

#define PI 3.14159265

DisplayWidget::DisplayWidget(DeepThought * thought, QRect &qr, QWidget *parent) : QGLWidget(QGLFormat(QGL::AlphaChannel|QGL::SampleBuffers|QGL::AccumBuffer), parent, 0, Qt::FramelessWindowHint)
{
  thinker=thought;

  timerId = startTimer(10);
  setGeometry(qr);
  setCursor(QCursor(Qt::BlankCursor)); //Hide the cursor
  //setCursor(QCursor(Qt::ArrowCursor)); //Show the cursor
  setPalette(QPalette(QColor(0, 0, 0)));
  setAutoFillBackground(false); //Try to let glClear work...
  delta = new QGLPixelBuffer(TEXTX, TEXTY, format(), this);
  #ifdef SHOWBAR
  bar = new QGLPixelBuffer(BAR_WIDTH, qr.height(), format(), this);
  #endif
  
  setWindowState(Qt::WindowFullScreen);
}

void DisplayWidget::keyPressEvent(QKeyEvent * event)
{
	event->accept();
	if (event->key()==Qt::Key_Escape) die();
}


DisplayWidget::~DisplayWidget()
{
  makeCurrent();
  glDeleteLists(shapeList,1);
}

void DisplayWidget::initializeGL()
{  
   deltaTexture = delta->generateDynamicTexture();
   delta->makeCurrent();
   glClearColor(0,0,0,1.0);
   glClear(GL_COLOR_BUFFER_BIT);
   
   delta->updateDynamicTexture(deltaTexture);
   
   #ifdef SHOWBAR
   barTexture = bar->generateDynamicTexture();
   bar->makeCurrent();
   glClearColor(0,0,0,1.0);
   glClear(GL_COLOR_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
   bar->updateDynamicTexture(deltaTexture);
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
   
   #ifdef SHOWBAR
   bar->makeCurrent();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 1, 0, 1, -1, 1);
   #endif
   
   update();
}

void DisplayWidget::paintGL()
{
	if(thinker->getReport()=="killMe") {thinker->die(); emit die();}
	#ifndef REALDATA 
	delta->makeCurrent();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	QPainter paint(delta);
	paint.setBrush(Qt::white);
	paint.setPen(Qt::white);
	//paint.setFont(QFont("Times",12));
	paint.drawText(8,TEXTY-2,thinker->getReport());
	paint.end();
	glFlush();
	delta->updateDynamicTexture(deltaTexture);
	#endif
	
	#ifdef SHOWBAR
	double error;
	if (thinker->getEndError(error))
	{
		bar->makeCurrent();
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(error,1.0-error,0,1);
		glRectf(0,0,1,1);
		glAccum(GL_MULT, .7);
		glAccum(GL_ACCUM, .3);
		glAccum(GL_RETURN, 1.0);
		glRectf(0,0,.5,1);
		glFlush();
		bar->updateDynamicTexture(barTexture);
	}
	#endif
	
	makeCurrent();
	
	Point<double> targetd=thinker->getTargetDest();
	Point<double> targeto=thinker->getTargetOrigin();
	Point<double> cursor=thinker->getCursor();
	Point<double> targetcolor=thinker->getTargetColor();
	Point<double> screencolor=thinker->getScreenColor();
	
	glClearColor(screencolor.X(), screencolor.Y(), screencolor.Z(),0);
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
	glColor3f (1.0, 1.0, 1.0);
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
	
	#ifdef SHOWBAR
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, barTexture);
	
	glBegin(GL_POLYGON);
		glTexCoord2d(0,1); glVertex2d(W/2-BAR_WIDTH,H/2); //Top left
		glTexCoord2d(0,0); glVertex2d(W/2-BAR_WIDTH,-H/2); //Bottom Left
		glTexCoord2d(1,0); glVertex2d(W/2,-H/2); //Bottom Right
		glTexCoord2d(1,1); glVertex2d(W/2,H/2); //Top Right
		glEnd();
	glDisable(GL_TEXTURE_2D);
	#endif
	
	//double * rotate=
	glRotated(thinker->getRotation(),0,0,1);
	
	glColor3f(targetcolor.X(),targetcolor.Y(), targetcolor.Z()); //Draw Target
	glPushMatrix();
	glTranslatef(targetd.X(),targetd.Y(),targetd.Z());
	glCallList(shapeList);
	glPopMatrix();
	
	if(targeto.dist(cursor)>RAD) glColor3f(1,1,0);
	else glColor3f(.75,.75,0);
	glPushMatrix();
	glTranslatef(targeto.X(),targeto.Y(),targeto.Z());
	glBegin(GL_POINTS);
		glVertex2d(0,0);
	glEnd();
	glPopMatrix();
	
	if (thinker->getHold()) glColor3f(0, 0, 1.0); //Draw Cursor
	else glColor3f(0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(cursor.X(),cursor.Y(),cursor.Z());
	glCallList(shapeList);
	glPopMatrix();
	
	//#define _SHOWARM
	#ifdef _SHOWARM
		Point<double> joint=thinker->getJoint();
		Point<double> end=thinker->getEnd();
		Point<double> origin=thinker->getOrigin();
		glColor3f(0, 1, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2d(origin.X(),origin.Y());
		glVertex2d(joint.X(),joint.Y());
		glVertex2d(end.X(),end.Y());
		glEnd();
	#endif
	
	//glFinish();  //Get precise timing
	//thinker->punchClock();
}

