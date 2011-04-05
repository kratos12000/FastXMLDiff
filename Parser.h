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
		QDomNode Comp_XML(QDomNode* old_doc, QDomNode* new_doc);

};
