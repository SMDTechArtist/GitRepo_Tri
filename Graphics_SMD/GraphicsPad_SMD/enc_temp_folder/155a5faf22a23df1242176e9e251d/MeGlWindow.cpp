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
#include <ctime>


using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;
//v1 = rand() % 100;         // v1 in the range 0 to 99
//v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014




const GLuint NUM_VERTICES_PER_POLY = 5;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint POLYGON_BYTE_SIZE = NUM_VERTICES_PER_POLY * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint shaderID;
GLuint TriIndexBufferID;
GLuint boundaryIndexBufferID;
//glm::vec3 

const float MOVEMENT_SPEED = 0.1f;
glm::vec3 velocity;



//glm::vec2 PolyPos(+0.0f, 0.0f); //Tri 1 position
//glm::vec2 tri2Pos(+0.1f, 0.1f); // Tri 2 position




struct Vertex
{
	glm::vec3 position;
	glm::vec3 Color;
};
		

namespace
{
	glm::vec3 PolyVerts[] = //was verts
	{
		glm::vec3(+0.0f, +0.1f, +1.0f),
		glm::vec3(+0.1f, +0.1f, +1.0f),
		glm::vec3(+0.15f, +0.0f, +1.0f),
		glm::vec3(-0.1f, -0.1f, +1.0f),
		glm::vec3(+0.0f, -0.1f, +1.0f),
	};

	GLushort PolyIndices[] = { 0,1,2,3,4 };

	glm::vec3 bounaryVerts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f), //0
		glm::vec3(-1.0f, +0.0f, +0.0f), //1
		glm::vec3(+0.0f, -1.0f, +0.0f), //2
		glm::vec3(+1.0f, +0.0f, +0.0f), //3



	};

	GLushort boundaryIndice[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	const unsigned int NUM_POLY_VERTS = sizeof(PolyVerts) / sizeof(*PolyVerts);
	const unsigned int NUM_BOUNDARY_VERTS = sizeof(bounaryVerts) / sizeof(*bounaryVerts);
	

	//Making Buffer Variables
	GLuint polyVertexBufferID;
	GLuint boundaryVertexBufferID;

	glm::vec3 transformedVerts[NUM_POLY_VERTS];
	glm::vec3 PolyPos(+0.0f, 0.0f, +0.0f);

}

void sendDataToOpenGL()
{
	
	//glEnableVertexAttribArray(0);

	//Buffer for Tri 
	glGenBuffers(1, &polyVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, polyVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PolyVerts), PolyVerts, GL_DYNAMIC_DRAW);
	
	//Buffer for Boundary 
	glGenBuffers(1, &boundaryVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bounaryVerts), bounaryVerts, GL_STATIC_DRAW);
	


	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	//glEnableVertexAttribArray(1);
	
	

	
	glGenBuffers(1, &TriIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PolyIndices), PolyIndices, GL_STATIC_DRAW);


	

	
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
	//vec3 offset(PolyPos.x, PolyPos.y, 0);
	

	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	//GLint offsetUniformLocation = glGetUniformLocation(programID, "offset");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//dominatingColor.r = 1;
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);

	

	//glBufferData(GL_ARRAY_BUFFER, sizeof(PolyVerts), NULL, GL_DYNAMIC_DRAW);// this should reattach the PolyVerts buffer to the ARRAY_BUFFER binding point
	
	

	
	glBindBuffer(GL_ARRAY_BUFFER, polyVertexBufferID);
	glm::vec3 translatedVerts[NUM_POLY_VERTS];
	for (unsigned int i = 0; i < NUM_POLY_VERTS; i++)
	{
		translatedVerts[i] = PolyVerts[i] + PolyPos;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(translatedVerts), translatedVerts);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Add late if something doesn't work right. But makes Tri weird right now.
	glDrawArrays(GL_POLYGON, 0, 5);

	

	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);

	
}


void MeGlWindow::myUpdate()
{
	//glm::vec3 velocity(rand() % 100 * 0.00001, rand() % 100 * 0.0001f, +0.0f); //Change the num of zeros to slow or speed the velocity of tri1
	PolyPos = PolyPos + velocity;
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

int randSign()
{
	return rand() % 2 == 0 ? 1 : -1; // this makes it so that our random number goes into quodrents other then just (+,+)
	//it says for the random number generated (rand()) do a mod 2 (% 2) if the mod to is == to 0 then return +1 otherwise (?) return -1
}

float randComponent()
{
	return rand() % 100 * 0.000001 * randSign();
}
// depending on what randSign() returns, the random function is X by +1 or -1.


const vec3 first = bounaryVerts[0];
const vec3 second = bounaryVerts[1];
//dotProduct
int dotProduct(int first[], int second[])
{
	int product = 0; 

	for (int i = 0; i < sizeof(bounaryVerts); i++)
		product = product + first[i] * second[i];
	return product;

	cout << "product is: ";
	cout << product;
	exit(1);
}


void MeGlWindow::handleBoundaries()
{

	vec3 wall = second - first;
	//vec3 normal = wall.perpCcwXy();
	vec3 respectiveShipPosition = PolyPos - first;
	//float dotResult = normal.dot(respectiveShipPosition);
	//qDebug() << dotResult;


	//vec3 normal = wall.perpCcwXy();
	//https://www.youtube.com/watch?v=plpXkJg8oDc&list=PLRwVmtr-pp04XomGtm-abzb-2M1xszjFx&index=144	//Maybe have to watch math videos before setting this up. 	// Figure out what the equivelent of perpCcwXy (Perpendicular clockwise on the XY) is or what library to find it in. 
}

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();


	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(0);

	srand(time(0));
	
	float floaty = randComponent(); //used for bug checking to see what the value of our RandComponent is. 

	velocity = glm::vec3(randComponent(), randComponent(), +0.0f); //Change the num of zeros to slow or speed the velocity of tri1

}



void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		PolyPos.y += MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_S:
		PolyPos.y -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_A:
		PolyPos.x -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_D:
		PolyPos.x += MOVEMENT_SPEED;
		break;
	}
	repaint();
}


//Math variables
//glm::vec3 first;
//glm::vec3 second;
//glm::vec3 vecSubResult = first - second;

//Perpendicularity
//glm::vec3 meVector;
/*const vec3 perpCwXy()
{
	return vec3(y, -x, +0.0);
}
const vec3 perpCcwXy()
{
	return vec3(-y, x, +0.0);
}
const vec3 perpCw()
{
	return meVector.perpCwXy();
};
const vec3 perpCcw()
{
	return meVector.perpCCwXy();
};*/

// DotProduct
//const dotProduct()
//{
	//static::dot(first, second, +0.0);
//}
//const dot
//{
//	return fist.x * second.x + first.y * second.y + fist.z * second.z;
//}

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

