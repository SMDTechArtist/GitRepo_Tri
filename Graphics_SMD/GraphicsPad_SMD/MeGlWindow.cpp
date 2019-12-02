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
#include <Qt\qimagereader.h>





using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;
using glm::mat4;

using Timing::Clock;

const uint NUM_VERTICES_PER_TRI = 3;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint theBufferID;
GLuint programID;

GLuint cubeNumIndices;
GLuint arrowNumIndices;
GLuint teapotNumIndices;
GLuint planeNumIndices;

Camera camera;
GLuint fullTransformationUniformLocation;

GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint teapotVertexArrayObjectID;
GLuint planeVertexArrayObjectID;


GLuint cubeIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint teapotIndexByteOffset;
GLuint planeIndexByteOffset;


float ROTATION_SPEED = 30.0f;



void MeGlWindow::sendDataToOpenGL()
{
	Clock clock;

	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData plane = ShapeGenerator::makePlane();
	

	float shapeBufferVertIndexSize = cube.vertexBufferSize() + cube.indexBufferSize();
/*
		+
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize();
*/

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, shapeBufferVertIndexSize, 0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	//currentOffset += cube.indexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	//currentOffset += arrow.vertexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	//currentOffset += arrow.indexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	//currentOffset += teapot.vertexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	//currentOffset += teapot.indexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	//currentOffset += plane.vertexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	//currentOffset += plane.indexBufferSize();

	// to add more shapes you will need to repeat the above process

	cubeNumIndices = cube.numIndices;
	arrowNumIndices = arrow.numIndices;
	teapotNumIndices = teapot.numIndices;
	planeNumIndices = plane.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	//glGenVertexArrays(1, &planeVertexArrayObjectID);


	//geo shape 1
	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3); // texture Pointer?
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0); //Cube starts at 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3)); //Color data
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 9));//the Texture pointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cubeIndexByteOffset = cube.vertexBufferSize();

	/*
	//geo Shape 2
	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3); // texture Pointer?
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0); //Plane starts at 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3)); //Color data
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 9));//the Texture pointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	planeIndexByteOffset = plane.vertexBufferSize();
	*/

	cube.cleanup();
	//plane.cleanup();


	
}



int debugCount = 0;



void MeGlWindow::paintGL()
{
	Clock clock;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());


	auto vd = camera.viewDirection;glm::rotate(ROTATION_SPEED, vec3(1.0f, 0.0f, 0.0f));
	std::cout << vd.x << " " << vd.y << " " << vd.z << std::endl;

	

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLint modelToWorldMatrixUniformLocation =
		glGetUniformLocation(programID, "modelToWorldMatrix");

	//Ambiant Light
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.08f, 0.08f, 0.08f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);


	//camera Position here
	
	GLint cameraPosWorldUniformLocation = glGetUniformLocation(programID, "cameraPosition");
	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(cameraPosWorldUniformLocation, 1, &cameraPosition[0]);

	//Light Position here
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPosition");
	glm::vec3 lightPosition(0.0f, 0.5f, 0.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// Load Texture file
	const char * texName = "z:/1_School/Texture/Cat_Texture_S.png";
	QImage textureImge =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));

	//Copy file to OpenGL
	glActiveTexture(GL_TEXTURE);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImge.width(),
		textureImge.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		textureImge.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	//Set the Tex1 sampler uniform to refer to texture unit 0
	int textureUniformLocation = glGetUniformLocation(programID, "Tex1");

	if (textureUniformLocation >= 0)
		glUniform1i(textureUniformLocation, 0);
	else
		fprintf(stderr, "Uniform variable Tex1 not found !\n");


	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cube1ModelToWorldMatrix =
		glm::translate(vec3(-2.0f, 0.0f, -5.0f)) *
		glm::rotate(ROTATION_SPEED, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cube1ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);


	/*
	//plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 plane1ModelToWorldMatrix =
		glm::translate(vec3(-2.0f, 0.0f, -5.0f)) *
		glm::rotate(0.0f, vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * plane1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&plane1ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
	*/


	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(16);

	update();

	clock.newFrame();

}


void MeGlWindow::myUpdate()
{
	Clock clock;
	ROTATION_SPEED += 0.2f;
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
	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	
}

MeGlWindow::~MeGlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}





