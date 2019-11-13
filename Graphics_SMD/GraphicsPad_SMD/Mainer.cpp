#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <glfw3.h>
#include <QtGui\qwidget.h>


int main(int argc, char* argv[])
{
	//GLFWwindow* window = glfwCreateWindow();
	QApplication app(argc, argv);
	MeGlWindow meWindow;
	meWindow.show();
	//QWidget meWidg;
	//meWidg.show();
	return app.exec();


}