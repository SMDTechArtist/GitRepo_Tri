#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <gl\glew.h>
#include <QtOpenGL\qglwidget>
#include <string>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>





class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void keyPressEvent(QKeyEvent*);

public:
};

#endif