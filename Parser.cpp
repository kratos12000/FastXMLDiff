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

	Parser::Comp_XML(&old_doc, &new_doc);

	return 0;	
}

QDomNode Parser::Comp_XML(QDomNode* old_doc, QDomNode* new_doc)
{
	QDomNode old_child = *old_doc;
	QDomNode new_child = *new_doc;
	QDomComment added, removed, modified;
	added.setData("Node Added");
	removed.setData("Node Deleted");
	modified.setData("Node Changed");

	while (old_child.hasChildNodes() == true)
	{

		if(old_child.hasChildNodes() == true && new_child.hasChildNodes() == true) //If both nodes have children.
		{
			old_child = old_child.firstChild();
			new_child = new_child.firstChild();
		}
		
		else if(new_child.hasChildNodes() == false) //If the old_child has children and new_child doesn't.
		{
			new_child.appendChild(removed); //Append a removed node, then set the next children to be the current nodes.
			old_child = old_child.firstChild();
			new_child = new_child.firstChild();
		}
	}

	if(new_child.hasChildNodes() == true) //If new_child has more children.
	{
		new_child.appendChild(added); //Append an added node to the current child.
	}

	else if(old_child.nodeValue() != new_child.nodeValue()) //If the two have different contents.
	{
		new_child.appendChild(modified);
	}
	
	


//childNodes() function and length() function
	return new_child;
}
