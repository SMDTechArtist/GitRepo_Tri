#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>
#include <Vertex.h>

using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;


const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;


/*struct Vertex
{
	GLuint Tri_1
	{
		glm::vec2 tri1Position;
		glm::vec3 tri1Color;
	};

	GLuint Tri_2
	{
		glm::vec2 tri2Position;
		glm::vec3 tri2Color;
	};
};*/
		




/*struct Vertex
{

	//glm::vec3 position; //This works
	//glm::vec3 Color;  //This works

	glm::vec3 position; // Tri 1
	glm::vec3 Color; // Tri 1
	
	//glm::vec3 triangle2Position; // Tri 2
	//glm::vec3 triangle2Color; // Tri 2

};*/

struct moveUp
{

};

struct moveDown
{

};
struct moveLeft
{

};
struct moveRight
{

};



void sendDataToOpenGL()
{
	
	Vertex Triangle[] =
	{
		glm::vec3(+0.0f, +0.2f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(+0.2f, -0.2f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(-0.2f, -0.2f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};

	/*GLfloat Triangle[] =
	{
		+0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
	};*/

	

	GLuint TriangleBufferID;
	glGenBuffers(1, &TriangleBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
	
	//Tri_1 Attribute Pointer, which points to the location 0 and 1 in vertex shader doc
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));

	//Tri_2 Attribute Pointer, which points to the location 2 and 3 in vertex shader doc
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));


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
vec2 TrianglePosition(0.0f, 0.0f);
/*void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		TrianglePosition.moveUp();
		break;
	case Qt::Key::Key_S:
		TrianglePosition.moveDown();
		break;
	case Qt::Key::Key_A:
		TrianglePosition.moveULeft();
		break;
	case Qt::Key::Key_D:
		TrianglePosition.moveURight();
		break;
	}
}*/

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_W)
	{
		TrianglePosition.y += 1;
	}
	if (e->key() == Qt::Key_S)
	{
		TrianglePosition.y += 1;
	}
	if (e->key() == Qt::Key_D)
	{
		TrianglePosition.y += 1;
	}
	if (e->key() == Qt::Key_A)
	{
		TrianglePosition.y += 1;
	}
}

/*void MeGlWindow::myUpdate()
{
	clock.newFram();
	repaint();
};*/
//Help

