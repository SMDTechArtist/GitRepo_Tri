#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <glfw3.h>
#include <MeWidget.h>


int main(int argc, char* argv[])
{
	//GLFWwindow* window = glfwCreateWindow();
	QApplication app(argc, argv);
	MeWidget widg;
	widg.show();
	//MeGlWindow meWindow;
	//meWindow.show();
	return app.exec();


}