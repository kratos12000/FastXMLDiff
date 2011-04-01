#include "MainWindow.h"
#include "DiffWidget.h"
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	mainWidget = new DiffWidget;
	mainWidget->show();
	connect (mainWidget,SIGNAL(canceled()),this,SLOT(close()));
	setCentralWidget(mainWidget);
}
