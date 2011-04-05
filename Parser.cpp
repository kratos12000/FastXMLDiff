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
	QDomDocument retDoc;
	added = "Node Added";
	removed = "Node Deleted";
	modified = "Node Changed";
	
        if(length(childNodes(new_child)) == length(childNodes(old_child)))
	{
		if(isNull(firstChild(new_child)) != true)
		{
			if(nodeValue(firstChild(new_child)) != nodeValue(firstChild(old_child)))
			{
				//Set attribute to modified.
			}
			Comp_XML(firstChild(old_child), firstChild(new_child));
		
			int nodeLen = length(childNodes(new_child));
			new_child = firstChild(new_child);
			old_child = firstChild(old_child);
			for(int x = (nodeLen-1); x>0; x--)
			{
				if(nodeValue(nextSibling(new_child)) != nodeValue(nextSibling(old_child)))
                        	{
                        	        //Set attribute to modified.
                        	}
				Comp_XML(old_child, new_child);
				new_child = nextSibling(new_child);
				old_child = nextSibling(old_child);
			}
		}
	}
	
	old_child = old_doc;
	new_child = new_doc;

	if(length(childNodes(new_child)) != length(childNodes(old_child)))
	{
		if(length(childNodes(new_child)) > length(childNodes(old_child)))
		{
			if(isNull(firstChild(old_child)) != true)
			{
				if(nodeValue(firstChild(new_child)) != nodeValue(firstChild(old_child)))
				{
					//Set attribute to modified.
				}
				Comp_XML(firstChild(old_child), firstChild(new_child));
				
				int nodeLen = length(childNodes(new_child));
				new_child = firstChild(new_child);
				old_child = firstChild(old_child);
				for(int x = (nodeLen-1); x>0; x--)
				{
					if(isNull(nextSibling(old_child)) == true)
					{
						//Set attribute to added.
					}
					else if(nodeValue(nextSibling(new_child)) != nodeValue(nextSibling(old_child)))
					{
						//Set attribute to modified.
					}
					Comp_XML(old_child,new_child);
					new_child = nextSibling(new_child);
					old_child = nextSibling(old_child);
				}

			}
		}
	
	
		else
		{
	
                        if(isNull(firstChild(new_child)) != true)
                        {
                                if(nodeValue(firstChild(old_child)) != nodeValue(firstChild(new_child)))
                                {
                                        //Set attribute to modified.
                                }
                                Comp_XML(firstChild(old_child), firstChild(new_child));

                                int nodeLen = length(childNodes(old_child));
                                new_child = firstChild(new_child);
                                old_child = firstChild(old_child);
                                for(int x = (nodeLen-1); x>0; x--)
                                {
                                        if(isNull(nextSibling(new_child)) == true)
                                        {
                                                //Set attribute to deleted.
                                        }
                                        else if(nodeValue(nextSibling(old_child)) != nodeValue(nextSibling(new_child)))
                                        {
                                                //Set attribute to modified.
                                        }
                                        Comp_XML(old_child,new_child);
                                        new_child = nextSibling(new_child);
                                        old_child = nextSibling(old_child);
                                }

                        }
                }
	}
	retDoc = //The "new" node tree with the annotation elements.
	return retDoc;

/*
	while (old_child->hasChild() == true)
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
*/	
}
