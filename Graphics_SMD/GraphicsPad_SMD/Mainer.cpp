#include <iostream>
#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <glfw3.h>


int main(int argc, char* argv[])
{
	//GLFWwindow* window = glfwCreateWindow();
	QApplication app(argc, argv);
	MeGlWindow meWindow;
	meWindow.show();
	return app.exec();


}