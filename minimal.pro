######################################################################
# Automatically generated by qmake (2.01a) Sat Nov 22 17:22:58 2008
######################################################################

TEMPLATE = app
TARGET = minimal
DEPENDPATH += . 
INCLUDEPATH += /research/include
QT += opengl
CONFIG += debug
LIBS += -lHAPI -lH3DUtil -lm -L/research/include -ljcommon #-lcublas
#CUDA_DIR = $$system(which nvcc | sed 's,/bin/nvcc$,,')
#INCLUDEPATH += $$CUDA_DIR/include
#QMAKE_LIBDIR += $$CUDA_DIR/lib
 
# Input
HEADERS += devicesampler.h displaywidget.h experiment.h mousehandler.h targetcontrol.h devicehandler.h
SOURCES += main.cpp devicesampler.cpp displaywidget.cpp deepthought.cpp devicehandler.cpp targetcontrol.cpp pertrial.cpp
