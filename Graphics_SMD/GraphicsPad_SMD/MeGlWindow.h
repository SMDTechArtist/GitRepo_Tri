#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <gl\glew.h>
#include <QtOpenGL\qglwidget>
#include <string>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>
#include <cassert>
#include <Qt\qtimer.h>






class QKeyEvent;

class MeGlWindow : public QGLWidget
{
	Q_OBJECT
	QTimer myTimer;

protected:
	void initializeGL();
	//void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void sendDataToOpenGL();




private slots:
	//void myUpdate();
	void paintGL();

public:
	~MeGlWindow();

};

#endif
