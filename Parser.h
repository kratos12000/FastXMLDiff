#include <QDomDocument>
#include <QDialog>
#include <QtXml>
#include <iostream>
using namespace std;

class Parser : public QDialog
{
	private:
		QDomNode * m_node;

	public:
		int Read_XML(QString old_path, QString new_path);
		QDomDocument Comp_XML(QNode old_doc, QNode new_doc);

};
