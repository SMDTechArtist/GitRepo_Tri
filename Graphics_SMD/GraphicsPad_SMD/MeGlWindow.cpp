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
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <Qt\qapplication.h>





using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;




const GLuint NUM_VERTICES_PER_SHIP = 5;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint SHIP_BYTE_SIZE = NUM_VERTICES_PER_SHIP * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint shaderID;
GLuint TriIndexBufferID;
GLuint boundaryIndexBufferID;








void MeGlWindow::sendDataToOpenGL()
{
	ShapeData tri = ShapeGenerator::makeTriangle();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufferSize(), tri.indices, GL_STATIC_DRAW);
	tri.cleanup();
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundaryVerts), boundaryVerts, GL_STATIC_DRAW);
	

	

	
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

	

	GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominatingColor");
	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);

	glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);

	
	
	glBindBuffer(GL_ARRAY_BUFFER, ShipVertexBufferID);
	vec3 translatedVerts[NUM_SHIP_VERTS];
	for (unsigned int i = 0; i < NUM_SHIP_VERTS; i++)
	{
		translatedVerts[i] = ShipVerts[i] + ShipPos;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(translatedVerts), translatedVerts);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 

	

	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);

	
}


void MeGlWindow::myUpdate()

	MeGlWindow QKeyEvent();


	
	repaint();

	handleBoundaries();


}


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

	glUseProgram(programID);

}

int randSign()
{
	return rand() % 2 == 0 ? 1 : -1; 
}

float randComponent()
{
	return rand() % 100 * 0.00001 * randSign();
}



void MeGlWindow::initializeGL()
{
	Clock clock;
	glewInit();
	sendDataToOpenGL();
	installShaders();

	GLenum errorCode = glewInit();
	assert(errorCode == 0);
	assert(clock.initialize());


	connect(&myTimer, SIGNAL(timeout()),
		this, SLOT(myUpdate()));
	myTimer.start(0);

	srand(time(0));
	
	float floaty = randComponent(); //used for bug checking to see what the value of our RandComponent is. 

	ShipVelocity = vec3(randComponent(), randComponent(), +0.0f); //Change the num of zeros to slow or speed the velocity of tri1

	
}

void MeGlWindow::checkBoundaries()
{
	if (ShipPos.x < -1 || ShipPos.x > 1)
		ShipVelocity.x *= -1;
	if (ShipPos.y < -1 || ShipPos.y > 1)
		ShipVelocity.y *= -1;
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		ShipVelocity.y += ACCELERATION;
		break;
	case Qt::Key::Key_S:
		ShipVelocity.y -= ACCELERATION;
		break;
	case Qt::Key::Key_A:
		ShipVelocity.x -= ACCELERATION;
		break;
	case Qt::Key::Key_D:
		ShipVelocity.x += ACCELERATION;
		break;
	}
	repaint();
}



glm::vec3 perpCcwXy(float x, float y)
{
	return glm::vec3(-y, x, 0.0f);

}


float magnitude(float x, float y, float z)
{
	return sqrt(x * x + y * y + z * z);
}



vec3 normalized(vec3 norm)
{
	
	float inverseMagnitude = 1.0f / magnitude(norm.x, norm.y, norm.z);
	vec3 IM(inverseMagnitude, inverseMagnitude, inverseMagnitude);
	
	vec3 normalizing(IM.x * norm.x, IM.y * norm.y, IM.z * norm.z);
	return normalizing;
}




void MeGlWindow::handleBoundaries()
{
	bool anyCollisions = false;
	for (uint i = 0; i < NUM_BOUNDARY_VERTS; i++)
	{
		const glm::vec3& first = boundaryVerts[i];
		const glm::vec3& second = boundaryVerts[(i + 1) % NUM_BOUNDARY_VERTS];
		
		glm::vec3 wall = second - first; //this gets us our perallel vector from the (0,0) to its respective equivelent

		glm::vec3 normal = perpCcwXy(wall.x, wall.y);
		glm::vec3 respectiveShipPosition = ShipPos - first;
		glm::vec3 normalized = glm::normalize(normal);
		float dotResult = glm::dot(respectiveShipPosition, normal);


		if (dotResult < 0)
		{
			ShipVelocity = ShipVelocity - 2 * glm::dot(ShipVelocity, normalized) * normalized;
			ShipPos = OldShipPos;
		}

	}
}



