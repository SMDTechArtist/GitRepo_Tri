#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
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
	/*GLfloat verts[] =
	{
		+0.0f, +0.0f,
		+1.0f, +0.0f, +1.0f,
		+0.5f, +0.0f,
		+0.0f, +0.0f, +1.0f,
		+0.0f, +0.5f,
		+0.0f, +0.0f, +1.0f,
		+0.5f, +0.5f,
		+0.0f, +0.0f, +1.0f,
		+0.0f, -0.5f,
		+1.0f, +0.0f, +1.0f,
		-0.5f, -0.5f,
		+1.0f, +0.0f, +1.0f,

	};*/


	GLfloat Triangle_1[] =
	{
		+0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
	};

	/*GLfloat Triangle_2[]
	{
		+0.5f, +0.5f,
		+0.0f, +0.0f, +1.0f,
		+0.0f, -0.5f,
		+1.0f, +0.0f, +1.0f,
		-0.5f, -0.5f,
		+1.0f, +0.0f, +1.0f,
	};
	*/

	//GLuint VertexBufferID;
	//glGenBuffers(1, &VertexBufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	/*GLuint NewVertexBufferID;
	glGenBuffers(1, &NewVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, NewVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
	*/

	GLuint Triangle_1ID;
	glGenBuffers(1, &Triangle_1ID);
	glBindBuffer(GL_ARRAY_BUFFER, Triangle_1ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle_1), Triangle_1, GL_STATIC_DRAW);
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



void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glViewport(0, 0, width(), height());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
}

/*void MeGlWindow::myUpdate()
{
	clock.newFram();
	repaint();
};*/
//Help

