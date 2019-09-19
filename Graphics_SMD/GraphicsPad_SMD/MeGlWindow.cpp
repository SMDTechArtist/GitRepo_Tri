#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>
//#include <Vertex.h>

using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;


//const float MOVEMENT_SPEED = 0.1f; //Distance Tirangle will move
GLuint numTris = 0; //number of triangles veriable

const GLuint NUM_VERTICES_PER_TRI = 3;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;

const float MOVEMENT_SPEED = 0.1f;
static const glm::vec3 Triangle1Movement(0.0f, 0.0f, -1.0f);
static const glm::vec3 UP(0.0f, 1.0f, 0.0f);


glm::vec3 tri1Pos; //Tri 1 position
glm::vec3 tri2Pos; // Tri 2 position
//glm::vec3 triangle2Color; // Tri 2 color

struct Vertex
{
	glm::vec3 position;
	glm::vec3 Color;
};
		
//





void sendDataToOpenGL()
{
	Vertex Triangles[] = //was verts
	{
		glm::vec3(-0.25f, +0.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),
		
		glm::vec3(+0.25f, +0.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
		 
		glm::vec3(+0.0f,  -0.5f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 2));

	GLushort indices[] = { 0,1,2 };

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*GLuint TriangleBufferID;
	glGenBuffers(1, &TriangleBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleBufferID);
	glBufferData(GL_ARRAY_BUFFER, 10000, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));
	*/


	

	//GLushort indices[] = { 0,5,4, 2,1,3 };




	/*Tris trisPosColor[] =
	{
		glm::vec2(+0.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec2(+0.5f, +0.5f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
	};

	//trisPosColor buffer
	GLuint trisPosColorBufferID;
	glGenBuffers(1, &trisPosColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, trisPosColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trisPosColor), trisPosColor, GL_STATIC_DRAW);

	//twoTris Attribute pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));
	*/


}

/*void sendAnotherTriToOpenGL() //Triangle 2?
{
	
	const GLfloat THIS_TRI_X = -1 + numTris * MOVEMENT_SPEED; 
	//this generates new triangles.
	//-1 is added to NUM_Tri verytime. Then X_ Delta moves that triangel over .1f
	//X_Delta could be replaced with Triangle Position for KeyMovement?

	GLfloat thisTri[] =
	{
		0.0f, +1.0f, 0.0f,
		//THIS_TRI_X, 0.0f, 0.0f, //triangle point position. By using THIS_TRI_X we can move the triangle over in the X. could use this for key input movement.
		+0.0f, +1.0f, +0.0f, //Color info green

		-1.0f, -1.0f, +0.0f,
		//THIS_TRI_X + MOVEMENT_SPEED, 0.0f, 0.0f, //triangle point position
		+0.0f, +1.0f, +0.0f, //Color info green

		+1.0f, -1.0f, +0.0f,
		//THIS_TRI_X, 0.0f, 0.0f, //triangle point position
		+0.0f, +1.0f, +0.0f, //Color info green
	};
	glBufferSubData(GL_ARRAY_BUFFER,
		numTris * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri),
}*/


void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);


	//glViewport(0, 0, width(), height());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
}


string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())tri1Pos += MOVEMENT_SPEED * Triangle1Movement;
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





void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		tri1Pos.y += MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_S:
		tri1Pos.y -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_A:
		tri1Pos.x -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_D:
		tri1Pos.x += MOVEMENT_SPEED;
	}
	repaint();
}


/*void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_W)
	{
		Triangle1Position.y += 1;
	}
	if (e->key() == Qt::Key_S)
	{
		Triangle1Position.y += 1;
	}
	if (e->key() == Qt::Key_D)
	{
		Triangle1Position.y += 1;
	}
	if (e->key() == Qt::Key_A)
	{
		Triangle1Position.y += 1;
	}
}*/

/*void MeGlWindow::myUpdate()
{
	clock.newFram();
	repaint();
};*/
//Help

