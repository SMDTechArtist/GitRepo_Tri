#include <iostream>
#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <glfw3.h>


int main(int argc, char* argv[])
{
	//GLFWwindow* window = glfwCreateWindow();
	QApplication application(argc, argv);
	MeGlWindow meGlWindow;
	if (!meGlWindow.initialize())
		return -1;
	meGlWindow.show();
	int errorCode = application.exec();
	if (!meGlWindow.shutdown())
		errorCode |= 1;
	return errorCode;



}