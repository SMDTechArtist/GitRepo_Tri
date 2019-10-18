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
#include <Clock.h>
#include <Qt\qapplication.h>


using Timing::Clock;


using namespace std;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;


//using std::cout;
//using std::endl;
//v1 = rand() % 100;         // v1 in the range 0 to 99
//v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014

//using Math::vec3;


const GLuint NUM_VERTICES_PER_SHIP = 5;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
const GLuint SHIP_BYTE_SIZE = NUM_VERTICES_PER_SHIP * NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint shaderID;
GLuint TriIndexBufferID;
GLuint boundaryIndexBufferID;

/*int TEST(int argc, char* argv[])
{
	LARGE_INTEGER clockFrequency;
	QueryPerformanceFrequency(&clockFrequency);

	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&startTime);
	qDebug() << "Hello";
	QueryPerformanceCounter(&endTime);

	LARGE_INTEGER delta;
	delta.QuadPart = endTime.QuadPart - startTime.QuadPart;

	QApplication application(argc, argv);
	MeGlWindow meGlWindow;
	meGlWindow.show();
	return application.exec();


}*/
//Math::vec3 


//const float MOVEMENT_SPEED = 0.1f * clock.timeElapsedLastFrame();


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
	vec3 ShipVelocity(+0.05f, 0.05f, +0.05f);
	Clock clock;
	const float ACCELERATION = 0.1f * clock.timeElapsedLastFrame(); //ACCELERATION was MOVEMENT_SPEED before
	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Add late if something doesn't work right. But makes Tri weird right now.
	glDrawArrays(GL_POLYGON, 0, 5);

	

	glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);

	
}


void MeGlWindow::myUpdate()
{
	Clock clock;
	clock.newFrame();
	MeGlWindow QKeyEvent();
	//updateVelocity();
	//ShipVelocity = vec3(0.005f, 0.005f, 0.0f); // May need to move this below the random veriable
	ShipPos = ShipPos + ShipVelocity * clock.timeElapsedLastFrame();
	//ShipPos = ShipPos + velocity;
	repaint();
	
	//Clock.lap();
	handleBoundaries();
	//ShipPos += ShipVelocity * clock.lastLapTime();
	//const float ACCELERATION = 0.3f * clock.lastLapTime();

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

//void MeGlWindow::checkBoundaries()
//{
//	if (ShipPos.x < -1 || ShipPos.x > 1)
//		ShipVelocity.x *= -1;
//	if (ShipPos.y < -1 || ShipPos.y > 1)
//		ShipVelocity.y *= -1;
//}

/*void MyGlWindow::updateVelocity()
{
	const float ACCELERATION = 0.3f * clock.lastLapTime();

	vec3 directionToAccelerate(-sin(shipOrientation), cos(shipOrientation));
	if(GetAsyncKeyState(VK_UP))
		shipVelocity += directionToAccelerate * ACCELERATION
}*/
glm::vec3 perpCcwXy(float x, float y)
{
	return glm::vec3(-y, x, 0.0f);

}


float magnitude(float x, float y, float z)
{
	return sqrt(x * x + y * y + z * z);
}

//float inverseMagnitude(vec3 mag)
//{
//	return 1.0f / magnitude(mag.x, mag.y, mag.z);
//}



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
		const glm::vec3& first = bounaryVerts[i];
		const glm::vec3& second = bounaryVerts[(i + 1) % NUM_BOUNDARY_VERTS];
		
		glm::vec3 wall = second - first; //this gets us our perallel vector from the (0,0) to its respective equivelent

		glm::vec3 normal = perpCcwXy(wall.x, wall.y);
		glm::vec3 respectiveShipPosition = ShipPos - first;

		float dotResult = glm::dot(respectiveShipPosition, normal);
		//float dotResult = normal.dot(respectiveShipPosition);
		//anyCollisions = anyCollisions || (dotResult < 0);
		//if (anyCollisions || (dotResult < 0))
		//{
		//	ShipVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		//}
		//qDebug() << anyCollisions;

		if (dotResult < 0)
		{
			ShipVelocity = ShipVelocity - 2 * glm::dot(ShipVelocity, normal) * normal;
			//ShipPos = oldShipPos;
		}

	}
}



