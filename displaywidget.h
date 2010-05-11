#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QRect>
#include <QGLPixelBuffer>
#include "point.h"
#include "deepthought.h"

//#define SHOWBAR

class DisplayWidget : public QGLWidget
{
  Q_OBJECT 

 public:
   DisplayWidget(DeepThought * thought, QRect &qr, QWidget *parent=0);
   ~DisplayWidget();
   void initializeGL();
   void paintGL();
   void resizeGL(int w, int h);
   void timerEvent(QTimerEvent *) { update(); }
   void keyPressEvent(QKeyEvent * event);
   
 private:
   void drawGLCircle(double rad=RAD, int points=20);   
   GLuint shapeList, deltaList;
   int W, H, widgetW, widgetH;
   int timerId;
   DeepThought * thinker;
   QGLPixelBuffer * delta;
   GLuint deltaTexture;
   #ifdef SHOWBAR
   QGLPixelBuffer * bar;
   GLuint barTexture;
   #endif
   
 signals:
   void die();
};

#endif
