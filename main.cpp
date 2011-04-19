#include "DiffWidget.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DiffWidget *window = new DiffWidget;
	window->setWindowTitle("XML Diff");
	window->show();
	QObject::connect(window, SIGNAL(canceled()), &app, SLOT(quit()));
	app.exec();	

}
