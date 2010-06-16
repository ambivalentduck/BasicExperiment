#############################################################################
# Makefile for building: minimal
# Generated by qmake (2.01a) (Qt 4.6.2) on: Wed Jun 16 16:32:27 2010
# Project:  minimal.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile minimal.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I. -I../libjcommon -I/usr/X11R6/include -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/X11R6/lib -lHAPI -lH3DUtil -lm -L/home/web/Programming/libjcommon -ljcommon -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		devicesampler.cpp \
		displaywidget.cpp \
		experiment.cpp \
		targetcontrol.cpp \
		pertrial.cpp moc_displaywidget.cpp
OBJECTS       = main.o \
		devicesampler.o \
		displaywidget.o \
		experiment.o \
		targetcontrol.o \
		pertrial.o \
		moc_displaywidget.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		minimal.pro
QMAKE_TARGET  = minimal
DESTDIR       = 
TARGET        = minimal

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: minimal.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtOpenGL.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -unix -o Makefile minimal.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtOpenGL.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile minimal.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/minimal1.0.0 || $(MKDIR) .tmp/minimal1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/minimal1.0.0/ && $(COPY_FILE) --parents devicesampler.h displaywidget.h experiment.h mousehandler.h targetcontrol.h devicehandler.h .tmp/minimal1.0.0/ && $(COPY_FILE) --parents main.cpp devicesampler.cpp displaywidget.cpp experiment.cpp targetcontrol.cpp pertrial.cpp .tmp/minimal1.0.0/ && (cd `dirname .tmp/minimal1.0.0` && $(TAR) minimal1.0.0.tar minimal1.0.0 && $(COMPRESS) minimal1.0.0.tar) && $(MOVE) `dirname .tmp/minimal1.0.0`/minimal1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/minimal1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_displaywidget.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_displaywidget.cpp
moc_displaywidget.cpp: displaywidget.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) displaywidget.h -o moc_displaywidget.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp experiment.h \
		devicesampler.h \
		devicehandler.h \
		pertrial.h \
		targetcontrol.h \
		displaywidget.h \
		getExperimentParams.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

devicesampler.o: devicesampler.cpp devicesampler.h \
		devicehandler.h \
		mousehandler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o devicesampler.o devicesampler.cpp

displaywidget.o: displaywidget.cpp displaywidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o displaywidget.o displaywidget.cpp

experiment.o: experiment.cpp experiment.h \
		devicesampler.h \
		devicehandler.h \
		pertrial.h \
		targetcontrol.h \
		displaywidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o experiment.o experiment.cpp

targetcontrol.o: targetcontrol.cpp targetcontrol.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o targetcontrol.o targetcontrol.cpp

pertrial.o: pertrial.cpp pertrial.h \
		devicesampler.h \
		devicehandler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pertrial.o pertrial.cpp

moc_displaywidget.o: moc_displaywidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_displaywidget.o moc_displaywidget.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
