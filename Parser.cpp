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

	QDomDocument ret_doc;
	QDomElement ret_node = ret_doc.createElement("root");
	ret_doc.appendChild(ret_node);
	
	Parser::Comp_XML(old_doc, new_doc, ret_doc, ret_node);

	QString diffdoc = ret_doc.toString();	
	cout << qPrintable(diffdoc) << endl;

	return 0;	
}

void Parser::Comp_XML(QDomNode old_node, QDomNode new_node, QDomDocument ret_doc, QDomNode ret_node)
{
	int old_length, new_length, max_length;
	old_length = old_node.childNodes().length();
	new_length = new_node.childNodes().length();

	if (old_length < new_length)
		max_length = new_length;
	else
		max_length = old_length;

	qDebug() << "Comparing " << old_node.nodeName() << ":" << old_node.nodeValue() << " and " << new_node.nodeName() << ":" 
		<< new_node.nodeValue() << endl;

	for (int i=0; i < max_length; i++){
		QDomElement child_element;
		if (i>=old_length){
			QDomNode child_node = new_node.childNodes().item(i);
			child_element = child_node.cloneNode(true).toElement();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("added", true);
		}
		else if (i>= new_length){
			QDomNode child_node = old_node.childNodes().item(i);
			child_element = child_node.cloneNode(true).toElement();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("removed", true);
		}
		else{
			QDomNode child_node = old_node.childNodes().item(i);
			child_element = child_node.toElement();
			if (!child_element.isNull()){
				child_element = ret_doc.createElement(child_element.tagName());
			} 
			else{
				//TODO: Handle processing-instructions, comments, perhaps non-text-entities.
				child_element = ret_doc.createElement("entity-replacement");
			}	
			Comp_XML(old_node.childNodes().item(i), new_node.childNodes().item(i), ret_doc, child_element);
			if (child_node.nodeValue() != new_node.childNodes().item(i).nodeValue()){
				child_element.setAttribute("modified", true);
			}
		}
		ret_node.appendChild(child_element);
	}
}
