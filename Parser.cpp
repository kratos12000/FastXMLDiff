#include <QtGui>
#include <QtXml>
#include <iostream>
#include "Parser.h"

using namespace std;

int Parser::Read_XML(QString old_path, QString new_path)
{
	QFile old_file(old_path);
        	if (!old_file.open(QIODevice::ReadOnly)){
               		QMessageBox::warning(this, "ERROR", QString("Could not open the Old XML file.  Looked in ") + old_path);
               return -1;
           }
	QFile new_file(new_path);
		if (!new_file.open(QIODevice::ReadOnly)){
                	QMessageBox::warning(this, "ERROR", QString("Could not open the New XML file.  Looked in ") + new_path);
		}
	QDomDocument old_doc("Old XML");		
		if (!old_doc.setContent(&old_file)) {
	               old_file.close();
               		QMessageBox::warning(this, "ERROR:", "Could not parse Old XML file");
               		return -1;
          	}
	old_file.close();

	QDomDocument new_doc("New XML");
                if (!new_doc.setContent(&new_file)) {
                       new_file.close();
                        QMessageBox::warning(this, "ERROR:", "Could not parse Modified XML file");
                        return -1;
                }
	new_file.close();

	QDomDocument* ret_doc;
	Parser::Comp_XML(old_doc, new_doc, ret_doc);
	
	cout << ret_doc->toString().constData();

	return 0;	
}

void Parser::Comp_XML(QDomNode old_node, QDomNode new_node, QDomNode* ret_node)
{
	int old_length, new_length, max_length;
	old_length = old_node.childNodes().length();
	new_length = new_node.childNodes().length();

	if (old_length < new_length)
		max_length = new_length;
	else
		max_length = old_length;

	if (max_length == 0){
		return;
	}

	ret_node = new QDomElement();

	for (int i=0; i < max_length; i++){
		QDomElement* child_element;
		if (i>=old_length){
			QDomNode* child_node = new QDomNode;
			*child_node = new_node.childNodes().item(i).cloneNode(true);
			child_element = static_cast<QDomElement*>(child_node);
			if (!child_element){
				child_element = new QDomElement();
				child_element->appendChild(*child_node);
			}
			child_element->setAttribute("added", true);
		}
		else if (i>= new_length){
			QDomNode* child_node = new QDomNode;
			*child_node = old_node.childNodes().item(i).cloneNode(true);
			child_element = static_cast<QDomElement*>(child_node);
			if (!child_element){
				child_element = new QDomElement();
				child_element->appendChild(*child_node);
			}				
			child_element->setAttribute("removed", true);
		}
		else{
			child_element = new QDomElement();
			Comp_XML(old_node.childNodes().item(i), new_node.childNodes().item(i), (QDomNode*)child_element);
			if (old_node.childNodes().item(i).nodeValue() != new_node.childNodes().item(i).nodeValue()){
				child_element->setAttribute("modified", true);
			}
		}
		ret_node->appendChild(*child_element);
	}
}
