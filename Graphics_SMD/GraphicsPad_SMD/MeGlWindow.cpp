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
#include <Qt/qDebug.h>
#include <Vector3D.h>


using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;

//using std::cout;
//using std::endl;
//v1 = rand() % 100;         // v1 in the range 0 to 99
//v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014

using Math::vec3;


const GLuint NUM_VERTICES_PER_SHIP = 5;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint SHIP_BYTE_SIZE = NUM_VERTICES_PER_SHIP * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint shaderID;
GLuint TriIndexBufferID;
GLuint boundaryIndexBufferID;
//Math::vec3 

const float MOVEMENT_SPEED = 0.1f;
Math::vec3 velocity;



//glm::vec2 ShipPos(+0.0f, 0.0f); //Tri 1 position
//glm::vec2 tri2Pos(+0.1f, 0.1f); // Tri 2 position




struct Vertex
{
	vec3 position;
	vec3 Color;
};
		

namespace
{
	vec3 ShipVerts[] = //was verts
	{
		vec3(+0.0f, +0.1f, +1.0f),
		vec3(+0.1f, +0.1f, +1.0f),
		vec3(+0.15f, +0.0f, +1.0f),
		vec3(-0.1f, -0.1f, +1.0f),
		vec3(+0.0f, -0.1f, +1.0f),
	};

	//GLushort ShipIndices[] = { 0,1,2,3,4 };

	vec3 bounaryVerts[] =
	{
		vec3(+0.0f, +1.0f, +0.0f), //0
		vec3(-1.0f, +0.0f, +0.0f), //1
		vec3(+0.0f, -1.0f, +0.0f), //2
		vec3(+1.0f, +0.0f, +0.0f), //3



	};

	GLushort boundaryIndice[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

	const unsigned int NUM_SHIP_VERTS = sizeof(ShipVerts) / sizeof(*ShipVerts);
	const unsigned int NUM_BOUNDARY_VERTS = sizeof(bounaryVerts) / sizeof(*bounaryVerts);
	

	//Making Buffer Variables
	GLuint ShipVertexBufferID;
	GLuint boundaryVertexBufferID;

	vec3 transformedVerts[NUM_SHIP_VERTS];
	vec3 ShipPos(+0.0f, 0.0f, +0.0f);

}

void sendDataToOpenGL()
{
	
	//glEnableVertexAttribArray(0);

	//Buffer for Tri 
	glGenBuffers(1, &ShipVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ShipVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ShipVerts), ShipVerts, GL_DYNAMIC_DRAW);
	
	//Buffer for Boundary 
	glGenBuffers(1, &boundaryVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bounaryVerts), bounaryVerts, GL_STATIC_DRAW);
	


	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	//glEnableVertexAttribArray(1);
	
	

	
	//glGenBuffers(1, &TriIndexBufferID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriIndexBufferID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ShipIndices), ShipIndices, GL_STATIC_DRAW);


	

	
	glGenBuffers(1, &boundaryIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundaryIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boundaryIndice), boundaryIndice, GL_STATIC_DRAW);



}

int debugCount = 0;

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glm::vec3 dominatingColor(0.0f, 0.0f, 1.0f);
	//vec3 offset(ShipPos.x, ShipPos.y, 0);
	

	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	//GLint offsetUniformLocation = glGetUniformLocation(programID, "offset");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//dominatingColor.r = 1;
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform3fv(offsetUniformLocation, 1, &offset[0]);

	

	//glBufferData(GL_ARRAY_BUFFER, sizeof(ShipVerts), NULL, GL_DYNAMIC_DRAW);// this should reattach the ShipVerts buffer to the ARRAY_BUFFER binding point
	
	

	
	glBindBuffer(GL_ARRAY_BUFFER, ShipVertexBufferID);
	vec3 translatedVerts[NUM_SHIP_VERTS];
	for (unsigned int i = 0; i < NUM_SHIP_VERTS; i++)
	{
		translatedVerts[i] = ShipVerts[i] + ShipPos;
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
	
	ShipPos = ShipPos + velocity;
	repaint();
	handleBoundaries();


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
	return rand() % 100 * 0.00001 * randSign();
}
// depending on what randSign() returns, the random function is X by +1 or -1.




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

	velocity = vec3(randComponent(), randComponent(), +0.0f); //Change the num of zeros to slow or speed the velocity of tri1

}



void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		ShipPos.y += MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_S:
		ShipPos.y -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_A:
		ShipPos.x -= MOVEMENT_SPEED;
		break;
	case Qt::Key::Key_D:
		ShipPos.x += MOVEMENT_SPEED;
		break;
	}
	repaint();
}

void MeGlWindow::handleBoundaries()
{
	bool anyCollisions = false;
	for (uint i = 0; i < NUM_BOUNDARY_VERTS; i++)
	{
		vec3& first = bounaryVerts[i];
		vec3& second = bounaryVerts[(i + 1) % NUM_BOUNDARY_VERTS];
		vec3 wall = second - first; //this gets us our perallel vector from the (0,0) to its respective equivelent

		vec3 normal = wall.perpCcwXy();
		vec3 respectiveShipPosition = ShipPos - first;
		float dotResult = normal.dot(respectiveShipPosition);
		//anyCollisions = anyCollisions || (dotResult < 0);
		if (anyCollisions || (dotResult < 0))
		{
			velocity = vec3(0.0f, 0.0f, 0.0f);
		}
		qDebug() << anyCollisions;

		/*if (dotResult < 0)
		{
			ShipVelocity = ShipVelocity - 2 * ShipVelocity.dot(normal) * normal;
			ShipPos = oldShipPosition;
		}*/

	}

//Math variables
//Math::vec3 first;
//Math::vec3 second;
//Math::vec3 vecSubResult = first - second;

//Perpendicularity
//Math::vec3 meVector;
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

/*int dot(int first[], int second[])
{
	int dotP = 0;

	for (int i = 0; i < sizeof(bounaryVerts); i++)
		dotP = dotP + first[i] * second[i];
	return dotP;

	cout << "dot is: ";
	cout << dot;
	exit(1);
}*/
//const vec3 perpCcwXy()
//{
	//return vec3(-y, x, +0.0);
//}


/*const vec3 Perpendicularity()
{
	vec3 meVector (0, 0, 0);
	vec3 perpCw = meVector.perpCwXy();
	vec3 perpCcw = meVector.perpCcwXy();
}*/



/*
const vec3 perpCwXy()
{
	vec3 NegX(-1.0f, +1.0f, +1.0f);
	return vec3(wall.x * NegX.x, wall.y * NegX.y, wall.z * NegX.z);
}*/

//*** This is another solution from someone else*****
/*
void normalize(float* v)
{
	float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	for (int i = 0; i < 3; i++)
	{
		v[i] = v[i] / length;
	}
}


float* cross_product(float* a, float* b)
{
	float result[] =
	{
		a[1] * b[2] - a[2] * b[1], 
		-(a[0] * b[2] - a[2] * b[0]), 
		a[0] * b[1] * b[0]
	};
	normalize(result);
	return result;
}

float* calculate_normal(float* a, float* b, float* c)
{
	float x[] =
	{
		b[0] - a[0],
		b[1] - a[1],
		b[2] - a[2],
	};
	float y[] =
	{
		c[0] - a[0],
		c[1] - a[1],
		c[2] - a[2],
	};

	float* result = cross_product(x, y);
	return result;
}*/


//const vec3 normal;

//dotProduct
//float dot()
//{
//	return first.x * second.x + first.y * second.y + first.z * second.z;
//};

//Perpendicular clockwise and conunter clockwise
/*const vec3 perpCcwXy()
{

	vec3 NegY(+1.0f, -1.0f, +1.0f);
	return vec3(wall.y * NegY.y, wall.x * NegY.x, wall.z * NegY.z);
};*/
/*const vec3 perpCwXy()
{

	vec3 NegX(-1.0f, +1.0f, +1.0f);
	return vec3(wall.y * NegX.y, wall.x * NegX.x, wall.z * NegX.z);
};*/

//Math::vec3 first;
//Math::vec3 second;
//float result = first.dot(second);
//Math::vec3 wall;



	/*//vec3 normal = wall.perpCcwXy(); //this gives us a (+) or (-) depending on if we are on one side of the boundry or the other. 
	vec3 NegY(+1.0f, -1.0f, +1.0f);
	vec3 perpCcwXy(wall.y * NegY.y, wall.x * NegY.x, wall.z * NegY.z);// this gives the Perpendicular Counter Clockwise direction. It swaps x and y and gives the y a negative value. 
	//vec3 normal = perpCcwXy;
	vec3 normal(wall.x * perpCcwXy.x + wall.y * perpCcwXy.y + wall.z * perpCcwXy.z); //getting the dot product of wall and perpCcwXy. 


	vec3 respectiveShipPosition = ShipPos - first;
	vec3 RSP = respectiveShipPosition;
	
	//float dotResult = normal.dot(respectiveShipPosition);
	float dotResult = (RSP.x * normal.x + RSP.y * normal.y + RSP.z * normal.z);
	qDebug() << dotResult;*/

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


