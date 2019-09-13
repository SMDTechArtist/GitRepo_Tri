#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui\qkeyevent>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;





const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;


//struct Vector
//{
//	float x, y, z;
//};

//struct Vertex
//{
//	Vector position;
//	Vector Color;
//};

void sendDataToOpenGL()
{
	
	GLfloat Triangle[] =
	{
		+0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
	};

	

	GLuint TriangleID;
	glGenBuffers(1, &TriangleID);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	GLushort indices[] = { 0,1,2 };

	//GLushort indices[] = { 0,5,4, 2,1,3 };

	//GLuint indexBufferID;
	//glGenBuffers(1, &indexBufferID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	
	GLint dominatingColorUniformLocation = 
		glGetUniformLocation(programID, "dominatingColor");
	GLint yFlipUniformLocation =
		glGetUniformLocation(programID, "yFlip");
	vec3 dominatingColor(0.0f, 1.0f, 0.0f);


	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	glUniform1f(yFlipUniformLocation, 1.0f);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

	dominatingColor.g = 0;
	dominatingColor.b = 1;

	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	glUniform1f(yFlipUniformLocation, -1.0f);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
}

/*void MeGlWindow::keyPressEvent(QKeyEvent* e);
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		Triangle.moveUp();
		break;
	case Qt::Key::Key_S:
		Triangle.moveDown();
		break;
	case Qt::Key::Key_D:
		Triangle.strafeRight();
		break;
	case Qt::Key::Key_A:
		Triangle.strafeLeft();
		break;
	}
}*/



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
	string temp = readShaderCode("VertexShaderCode.glsl");
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

	glUseProgram(programID);

}


//keyboard input here??????

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();


}





/*void MeGlWindow::myUpdate()
{
	clock.newFram();
	repaint();
};*/
//Help

