#include <QtGui>
#include <QtXml>
#include <iostream>
#include "Parser.h"

using namespace std;

int Parser::Read_XML(QString old_path, QString new_path, QString filePath)
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

	QFile diffed_file(filePath); //Create a new XML file for the custom deck.
        if (!diffed_file.open(QIODevice::WriteOnly | QFile::Truncate)){
                QMessageBox::warning(this, "ERROR:", QString("Could not open file ") + filePath + " for writing.");
                return -1;
        }

	QTextStream out(&diffed_file);

	QString diffdoc = ret_doc.toString();	
	out << qPrintable(diffdoc) << endl;

	diffed_file.close();

	return 1;	
}

void Parser::Comp_XML(const QDomNode old_node, const QDomNode new_node, QDomDocument ret_doc, QDomNode ret_node)
{
/*
	QTextStream myStream(stdout);
	myStream << "Comparing: " << endl;
	myStream  << old_node << endl;
	myStream << " and " << endl;
	myStream << new_node << endl;
*/
	int old_length, new_length, max_length;
	old_length = old_node.childNodes().length();
	new_length = new_node.childNodes().length();

	if (old_length < new_length)
		max_length = new_length;
	else
		max_length = old_length;

	qDebug() << "Max Length is: " << max_length << endl;
	
	qDebug() << "Comparing " << old_node.nodeName() << ":" << old_node.nodeValue() << " and " << new_node.nodeName() << ":" 
		<< new_node.nodeValue() << endl;

	for (int i=0; i < max_length; i++){
		QDomNode child_node;

		//If new_node has more children than old_node, handle the extra children
		if ( i >= old_length){
			child_node = new_node.childNodes().item(i).cloneNode(true);
			QDomElement child_element = child_node.toElement();
			qDebug() << "Cloning: " <<  child_element.nodeName() << ":" << child_element.nodeValue();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("added", "true");
			ret_node.appendChild(child_element);
			qDebug() << "New Node Added: " << child_element.nodeName() << " : " << child_element.nodeValue() << endl;
		}
		//If old_node has more children than new_node, handle the extra children
		else if (i>= new_length){
			child_node = old_node.childNodes().item(i).cloneNode(true);
			QDomElement child_element = child_node.toElement();
			qDebug() << "Cloning: " <<  child_element.nodeName() << ":" << child_element.nodeValue();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("removed", "true");
			ret_node.appendChild(child_element);
			qDebug() << "Old Node Removed: " << child_element.nodeName() << " : " << child_element.nodeValue() << endl;
		}
		else{
			QDomNode old_child = old_node.childNodes().item(i);
			QDomNode new_child = new_node.childNodes().item(i);
			qDebug() << "Parent: " << old_node.nodeName();
			qDebug() << "Parent: " << new_node.nodeName();
			if (old_child.nodeType() != new_child.nodeType())
				qDebug() << "Node Type Mismatch" << endl;
			if (old_child.isNull()){
				qDebug() << "Old Child Null" << endl;
			}
			if (new_child.isNull()){
				qDebug() << "New Child Null" << endl;
			}
			Q_ASSERT(old_child.nodeType() == new_child.nodeType());
			Q_ASSERT(!old_child.isNull() && !new_child.isNull());

			if(old_child.isProcessingInstruction())
			{
				if(old_child.nodeValue() != new_child.nodeValue())
                                {
                                        child_node = ret_doc.createElement("ProcessingInstruction-conflict");
                                        child_node.toElement().setAttribute("modified", "true");
                                        QDomElement oldPI = ret_doc.createElement("old-ProcessingInstruction");
                                        oldPI.appendChild(old_child.cloneNode(true));
                                        child_node.appendChild(oldPI);
                                        QDomElement newPI = ret_doc.createElement("new-ProcessingInstruction");
                                        newPI.appendChild(new_child);
                                        child_node.appendChild(newPI);
                                        ret_node.appendChild(child_node);
                                }
                                else
                                        ret_node.appendChild(old_child.cloneNode(true));

			}
			else if(old_child.isComment())
			{
				if(old_child.nodeValue() != new_child.nodeValue())
                                {
                                        child_node = ret_doc.createElement("comment-conflict");
                                        child_node.toElement().setAttribute("modified", "true");
                                        QDomElement oldComment = ret_doc.createElement("old-comment");
                                        oldComment.appendChild(old_child.cloneNode(true));
                                        child_node.appendChild(oldComment);
                                        QDomElement newComment = ret_doc.createElement("new-comment");
                                        newComment.appendChild(new_child.cloneNode(true));
                                        child_node.appendChild(newComment);
                                        ret_node.appendChild(child_node);
                                }
                                else
                                        ret_node.appendChild(old_child.cloneNode(true));

			}
			else if(old_child.isCDATASection())
			{
				if(old_child.nodeValue() != new_child.nodeValue())
                                {
                                        child_node = ret_doc.createElement("CDATA-conflict");
                                        child_node.toElement().setAttribute("modified", "true");
                                        QDomElement oldCDATA = ret_doc.createElement("old-CDATA");
                                        oldCDATA.appendChild(old_child.cloneNode(true));
                                        child_node.appendChild(oldCDATA);
                                        QDomElement newCDATA = ret_doc.createElement("new-CDATA");
                                        newCDATA.appendChild(new_child.cloneNode(true));
                                        child_node.appendChild(newCDATA);
                                        ret_node.appendChild(child_node);
                                }
                                else
                                        ret_node.appendChild(old_child.cloneNode(true));
			}
			else if(old_child.isAttr())
			{
				QDomAttr old_attr = old_child.toAttr();
				QDomAttr new_attr = new_child.toAttr();
				if(old_attr.name() != new_attr.name() ||  old_child.nodeValue() != new_child.nodeValue())
				{
					child_node = ret_doc.createElement("attribute-conflict");
                                	child_node.toElement().setAttribute("modified", "true");
                                	QDomElement oldAttr = ret_doc.createElement("old-attribute");
                                	oldAttr.appendChild(old_child.cloneNode(true));
                                	child_node.appendChild(oldAttr);
                                	QDomElement newAttr = ret_doc.createElement("new-attribute");
                                	newAttr.appendChild(new_child.cloneNode(true));
                                	child_node.appendChild(newAttr);
                                	ret_node.appendChild(child_node);	
				}
				else
					ret_node.appendChild(old_child.cloneNode(true));
			}
			else if(old_child.isText())
			{
				qDebug() << "Old Value: " << qPrintable(old_child.nodeValue()) << " New Value: " << qPrintable(new_child.nodeValue()) << endl;
				if(old_child.nodeValue() != new_child.nodeValue())
				{
					child_node = ret_doc.createElement("entity-conflict");
					child_node.toElement().setAttribute("modified", "true");
					QDomElement oldText = ret_doc.createElement("old-text");
					oldText.appendChild(old_child.cloneNode(true));
					child_node.appendChild(oldText);
					QDomElement newText = ret_doc.createElement("new-text");
					newText.appendChild(new_child.cloneNode(true));
					child_node.appendChild(newText);
					ret_node.appendChild(child_node);
				}
				else
					ret_node.appendChild(old_child.cloneNode(true));
			
			}
			else if(old_child.isNull()){
				qDebug() << "NULL!" << endl;
			}
			else //The nodes are elements
			{
				QDomElement old_element = old_child.toElement();
				QDomElement new_element = new_child.toElement();
				QDomElement child_element;
				qDebug() << "Old Name: " << old_child.nodeName() << " New Name: " << new_child.nodeName() << " ";
				qDebug() << "Old Tag: " << old_element.tagName() << " New Tag: " << new_element.tagName() << endl;
				if (old_element.tagName() != new_element.tagName()){
					child_node = ret_doc.createElement("container-node");
					child_element = child_node.toElement();
					child_element.appendChild(old_child.cloneNode(true));
					old_element.setAttribute("deleted", "true");
					new_element.setAttribute("added", "true");
					child_element.appendChild(new_child.cloneNode(true));
					child_element.setAttribute("modified", "true");
				}
				else {
					QDomNamedNodeMap old_attrs = old_element.attributes();
					QDomNamedNodeMap new_attrs = new_element.attributes();

					unsigned int old_num_attrs = old_attrs.count();
					unsigned int new_num_attrs = new_attrs.count();

					unsigned int max_num_attrs = old_num_attrs < new_num_attrs ? new_num_attrs : old_num_attrs;

				
					bool attributes_are_different = false;
					for (unsigned int i=0;i<max_num_attrs;i++){
						if (i >= old_num_attrs || i >= new_num_attrs){
							attributes_are_different = true;
							break;
						}
						QDomNode old_attr = old_attrs.item(i);
						QDomNode new_attr = new_attrs.item(i);
						if (old_attr.nodeValue() != new_attr.nodeValue()){
							attributes_are_different = true;
							break;
						}	
					}

					child_node = ret_doc.createElement(old_element.tagName());
					child_element = child_node.toElement();
					for (unsigned int i=0;i<max_num_attrs;i++){
						QDomAttr attribute;
						attribute = old_attrs.item(i).toAttr();
						if (attribute.isNull()){
							attribute = new_attrs.item(i).toAttr();
						}
						child_element.setAttribute(attribute.name(), attribute.value());
					}
					Comp_XML(old_child, new_child, ret_doc, child_element);
					if (old_node.nodeValue() != new_node.nodeValue() || attributes_are_different){
						child_element.setAttribute("modified", "true");
					}
				}

				ret_node.appendChild(child_element);
			}
		}
	}
}
