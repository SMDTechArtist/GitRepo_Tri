#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui\qkeysequence.h>
#include <QtGui\qkeyevent>
#include <Qt\qevent.h>
#include <cassert>
#include <Qt\qtimer.h>
#include <ctime>
#include <Qt/qDebug.h>
#include <Vector3D.h>
#include <glm\gtc\matrix_transform.hpp>
#include <Vertex.h> //If act funny add <Primitives/..>
#include <ShapeGenerator.h> //If act funny add <Primitives/..>
#include <Qt\qapplication.h>
#include <glm\gtx\transform.hpp>
#include <Camera.h>
#include <QtGui/qmouseevent>
#include <Clock.h>




using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;
using glm::mat4;



using Timing::Clock;

const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;
GLuint teapotNumIndices;
GLuint arrowNumIndices;
GLuint planeNumIndices;
GLuint numIndices;
//GLuint fullTransformUniformLocation;

Camera camera;

GLuint theBufferID;

GLuint teapostVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArryObjectID;
GLuint teapotIndexDataByTeOffset;
GLuint arrowIndexDataByTeOffset;
GLuint planeIndexDataByTeOffset;









void MeGlWindow::sendDataToOpenGL()
{
	Clock clock;
	ShapeData shape = ShapeGenerator::makeArrow();
	
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); //layout Location
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1); //layout Location
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
	
	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	
	numIndices = shape.numIndices;
	shape.cleanup();


	GLuint transformationMatrixBufferID;
	glGenBuffers(1, &transformationMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);


	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 12));
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	
}



int debugCount = 0;

float ROTATION_SPEED = 30.0f;
//glm::mat4 cubeRotation = glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f));
//glm::mat4 oldCubeRotation;

void MeGlWindow::paintGL()
{
	Clock clock;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());


	auto vd = camera.viewDirection;
	std::cout << vd.x << " " << vd.y << " " << vd.z << std::endl;


	//Ambiant Light
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.3f, 0.3f, 0.3f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	//Light Position here
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPosition");
	vec3 lightPosition(0.0f, 3.0f, 0.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	//camera Position here
	GLint cameraPosWorldUniformLocation = glGetUniformLocation(programID, "cameraPosition");
	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(cameraPosWorldUniformLocation, 1, &cameraPosition[0]);

	GLint fullTransformationUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);
	mat4 fullTransforms[] =
	{
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(ROTATION_SPEED, vec3(1.0f, 0.0f, 0.0f)),
		//projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(126.0f, vec3(0.0f, 1.0f, 0.0f))
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0, 2);
	

	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(16);

	update();

	clock.newFrame();
	//MeGlWindow QKeyEvent();
}



void MeGlWindow::myUpdate()
{
	Clock clock;
	ROTATION_SPEED += 0.2f;
	//clock.newFrame();
	MeGlWindow QKeyEvent();

	repaint();
}

bool MeGlWindow::initialize()
{
	Clock clock;
	return clock.initialize();
}

bool MeGlWindow::shutdown()
{
	Clock clock;
	return clock.shutdown();
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	std::cout << "you got to move it move it" << std::endl;
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();

}
	


bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

//

bool checkShaderStatus(GLuint shaderID)
{

	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkProgramStatus(GLuint programID)
{

	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetProgramInfoLog(programID, linkStatus, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}


void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl"); //if this starts acting funny take it back one file "..\\VertexShaderCode.glsl"
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);


	if (!checkProgramStatus(programID))
		return;


	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);

}



void MeGlWindow::initializeGL()
{
	Clock clock;
	setMinimumSize(1200, 600);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	

}

MeGlWindow::~MeGlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}





