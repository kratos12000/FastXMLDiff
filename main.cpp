#include "DiffWidget.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DiffWidget *window;
	if (argc == 4){
		window = new DiffWidget(QString(argv[1]), QString(argv[2]), QString(argv[3]));
	}
	else if (argc == 3){
		window = new DiffWidget(QString(argv[1]), QString(argv[2]));
	}
	else{
		window = new DiffWidget;
	}
	window->setWindowTitle("XML Diff");
	window->show();
	QObject::connect(window, SIGNAL(canceled()), &app, SLOT(quit()));
	app.exec();	

}
