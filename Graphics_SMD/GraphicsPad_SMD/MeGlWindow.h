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
#include<MeWidget.h>





class QKeyEvent;

class MeGlWindow : public QGLWidget
{
	Q_OBJECT

		QTimer myTimer;
	void sendDataToOpenGL();
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	//std::string readShaderCode(const char* fileName);
	void installShaders();

protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	//void sendDataToOpenGL();




private slots:
	void myUpdate();



public:
	~MeGlWindow();
	//MeWidget();

	bool shutdown();
	bool initialize();


};

#endif
