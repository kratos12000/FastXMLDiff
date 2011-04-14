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

	qDebug() << "Comparing " << old_node.nodeName() << ":" << old_node.nodeValue() << " and " << new_node.nodeName() << ":" 
		<< new_node.nodeValue() << endl;

	int old_num_attrs, new_num_attrs, max_num_attrs;
	old_num_attrs = old_node.attributes().length();
	new_num_attrs = new_node.attributes().length();

	if (old_num_attrs < new_num_attrs)
		max_num_attrs = new_num_attrs;
	else
		max_num_attrs = old_num_attrs;

	for (int i=0; i < max_num_attrs; i++){
		//TODO: Attribute comparison like element comparision below.	
	}

	for (int i=0; i < max_length; i++){
		QDomNode child_node;

		//If new_node has more children than old_node, handle the extra children
		if ( i >= old_length){
			child_node = new_node.childNodes().item(i).cloneNode(true);
			QDomElement child_element = child_node.toElement();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("added", "true");
			ret_node.appendChild(child_element);
		}
		//If old_node has more children than new_node, handle the extra children
		else if (i>= new_length){
			child_node = old_node.childNodes().item(i).cloneNode(true);
			QDomElement child_element = child_node.toElement();
			if (child_element.isNull()){
				child_element = ret_doc.createElement("container");
				child_element.appendChild(child_node.cloneNode(true));
			}
			child_element.setAttribute("removed", "true");
			ret_node.appendChild(child_element);
		}
		else{
			QDomNode old_child = old_node.childNodes().item(i);
			QDomNode new_child = new_node.childNodes().item(i);
			Q_ASSERT(old_child.nodeType() == new_child.nodeType());

			if(old_child.isProcessingInstruction())
			{
				if(old_child.nodeValue() != new_child.nodeValue())
                                {
                                        child_node = ret_doc.createElement("ProcessingInstruction-conflict");
                                        child_node.toElement().setAttribute("modified", "true");
                                        QDomElement oldPI = ret_doc.createElement("old-ProcessingInstruction");
                                        oldPI.appendChild(old_child);
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
                                        oldComment.appendChild(old_child);
                                        child_node.appendChild(oldComment);
                                        QDomElement newComment = ret_doc.createElement("new-comment");
                                        newComment.appendChild(new_child);
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
                                        oldCDATA.appendChild(old_child);
                                        child_node.appendChild(oldCDATA);
                                        QDomElement newCDATA = ret_doc.createElement("new-CDATA");
                                        newCDATA.appendChild(new_child);
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
                                	oldAttr.appendChild(old_child);
                                	child_node.appendChild(oldAttr);
                                	QDomElement newAttr = ret_doc.createElement("new-attribute");
                                	newAttr.appendChild(new_child);
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
					oldText.appendChild(old_child);
					child_node.appendChild(oldText);
					QDomElement newText = ret_doc.createElement("new-text");
					newText.appendChild(new_child);
					child_node.appendChild(newText);
					ret_node.appendChild(child_node);
				}
				else
					ret_node.appendChild(old_child.cloneNode(true));
			
			}
			else
			{
				QDomElement old_element = old_child.toElement();
				QDomElement new_element = new_child.toElement();
				QDomElement child_element;
				
				if (old_element.tagName() == new_element.tagName()){
					child_node = ret_doc.createElement(old_element.tagName());

					child_element = child_node.toElement();
					Comp_XML(old_child, new_child, ret_doc, child_element);
					if (old_node.nodeValue() != new_node.nodeValue()){
						child_element.setAttribute("modified", "true");
					}
				}
				else{
					child_node = ret_doc.createElement("container-node");
					child_element = child_node.toElement();
					child_element.appendChild(old_child);
					child_element.appendChild(new_child);
					child_element.setAttribute("modified", "true");
				}
				ret_node.appendChild(child_element);
			}
		}
	}
}
