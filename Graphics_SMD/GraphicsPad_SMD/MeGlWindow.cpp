#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>
#include <cassert>
#include <Qt\qtimer.h>



using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;





const GLuint NUM_VERTICES_PER_TRI = 3;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint shaderID;
GLuint TriIndexBufferID;
GLuint boundaryIndexBufferID;


const float MOVEMENT_SPEED = 0.1f;



//glm::vec2 tri1Pos(+0.0f, 0.0f); //Tri 1 position
//glm::vec2 tri2Pos(+0.1f, 0.1f); // Tri 2 position


struct Vertex
{
	glm::vec3 position;
	glm::vec3 Color;
};
		

namespace
{
	glm::vec3 triangleVerts[] = //was verts
	{
		glm::vec3(+0.1f, +0.14f, +1.0f),

		glm::vec3(+0.15f, +0.0f, +1.0f),

		glm::vec3(+0.0f,  -0.15f, +1.0f),
	};

	GLushort triIndices[] = { 0,1,2 };

	glm::vec3 bounaryVerts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f), //0
		glm::vec3(-1.0f, +0.0f, +0.0f), //1
		glm::vec3(+0.0f, -1.0f, +0.0f), //2
		glm::vec3(+1.0f, +0.0f, +0.0f), //3



	};

	GLushort boundaryIndice[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	const unsigned int NUM_TRI_VERTS = sizeof(triangleVerts) / sizeof(*triangleVerts);
	const unsigned int NUM_BOUNDARY_VERTS = sizeof(bounaryVerts) / sizeof(*bounaryVerts);
	

	//Making Buffer Variables
	GLuint triVertexBufferID;
	GLuint boundaryVertexBufferID;

	glm::vec3 transformedVerts[NUM_TRI_VERTS];
	glm::vec3 tri1Pos(+0.0f, 0.0f, +0.0f);

}

void sendDataToOpenGL()
{
	
	//glEnableVertexAttribArray(0);

	//Buffer for Tri 
	glGenBuffers(1, &triVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, triVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_DYNAMIC_DRAW);
	
	//Buffer for Boundary 
	glGenBuffers(1, &boundaryVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bounaryVerts), bounaryVerts, GL_STATIC_DRAW);
	


	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	//glEnableVertexAttribArray(1);
	
	

	
	glGenBuffers(1, &TriIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndices), triIndices, GL_STATIC_DRAW);


	

	
	glGenBuffers(1, &boundaryIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundaryIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boundaryIndice), boundaryIndice, GL_STATIC_DRAW);



}

int debugCount = 0;

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	vec3 dominatingColor(0.0f, 0.0f, 1.0f);
	//vec3 offset(tri1Pos.x, tri1Pos.y, 0);
	

	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	//GLint offsetUniformLocation = glGetUniformLocation(programID, "offset");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//dominatingColor.r = 1;
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);

	

	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), NULL, GL_DYNAMIC_DRAW);// this should reattach the triangleVerts buffer to the ARRAY_BUFFER binding point
	
	

	
	glBindBuffer(GL_ARRAY_BUFFER, triVertexBufferID);
	glm::vec3 translatedVerts[NUM_TRI_VERTS];
	for (unsigned int i = 0; i < NUM_TRI_VERTS; i++)
	{
		translatedVerts[i] = triangleVerts[i] + tri1Pos;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(translatedVerts), translatedVerts);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Add late if something doesn't work right. But makes Tri weird right now.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	

	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);
}


void MeGlWindow::myUpdate()
{
	glm::vec3 velocity(0.0001f, 0.0001f, +0.0f); //Change the num of zeros to slow or speed the velocity of tri1
	tri1Pos = tri1Pos + velocity;
	repaint();
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

/*string checkNumAttribs( GLuint checkAttribs)
{
	GLint numAttribs;
	glGetProgramInterfaceiv(checkAttribs, GL_ACTIVE_ATTRIBUTES, &numAttribs);

	GLenum Properties[] = 
	{
		GL_
	}
}*/

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

	glUseProgram(programID);

}



void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();

	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(0);
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
		break;
	}
	repaint();
}

void MeGlWindow::handleBoundaries()
{
	const vec3 first = bounaryVerts[0];
	const vec3 second = bounaryVerts[1];

	vec3 wall = second - first;
	//vec3 normal = wall.perpCcwXy();
	//https://www.youtube.com/watch?v=plpXkJg8oDc&list=PLRwVmtr-pp04XomGtm-abzb-2M1xszjFx&index=144	//Maybe have to watch math videos before setting this up. 	// Figure out what the equivelent of perpCcwXy (Perpendicular clockwise on the XY) is or what library to find it in. 
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

