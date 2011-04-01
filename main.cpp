#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow *window = new MainWindow;
	window->setWindowTitle("XML Diff");
	window->show();
	app.exec();	

}
