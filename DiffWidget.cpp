#include <QApplication>
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>
#include <iostream>
#include <QFileInfo>
#include "DiffWidget.h"
#include "Parser.h"

DiffWidget::DiffWidget(QString fname1, QString fname2, QString output, QWidget *parent) : QDialog(parent)
{
	init();
	browseLineEditOld->setText(fname1);
	browseLineEditNew->setText(fname2);
	nameLineEdit->setText(output);
}

DiffWidget::DiffWidget(QString fname1, QString fname2, QWidget *parent) : QDialog(parent)
{
	init();
	browseLineEditOld->setText(fname1);
	browseLineEditNew->setText(fname2);
}

DiffWidget::DiffWidget(QWidget *parent) : QDialog(parent)
{
	init();
}

void DiffWidget::init()
{
        nameLabel = new QLabel(tr("Output File Name:"));
        nameLineEdit = new QLineEdit;
        nameLabel->setBuddy(nameLineEdit);

        browseLabelOld = new QLabel(tr("Old XML File:"));
        browseLineEditOld = new QLineEdit;
        browseLabelOld->setBuddy(browseLineEditOld);
	
	browseLabelNew = new QLabel(tr("New XML File:"));
        browseLineEditNew = new QLineEdit;
        browseLabelNew->setBuddy(browseLineEditNew);

        browseButtonOld = new QPushButton(tr("&Browse"));
        browseButtonOld->setDefault(true);
//        browseButtonOld->setEnabled(false);

	browseButtonNew = new QPushButton(tr("&Browse"));
        browseButtonNew->setDefault(true);
//        browseButtonNew->setEnabled(false);

        okButton = new QPushButton(tr("&OK"));
	okButton->setEnabled(false);

        cancelButton = new QPushButton(tr("Cancel"));

        connect(browseLineEditOld, SIGNAL(textChanged(const QString &)),
                this, SLOT(enableOkButton(const QString &)));

	connect(browseLineEditNew, SIGNAL(textChanged(const QString &)),
		this, SLOT(enableOkButton(const QString &)));

        connect(browseButtonOld, SIGNAL(clicked()),
                this, SLOT(getOldPath()));
	connect(browseButtonNew, SIGNAL(clicked()),
                this, SLOT(getNewPath()));
        connect(okButton, SIGNAL(clicked()),
                this, SLOT(applyButtonPressed()));
        connect(cancelButton, SIGNAL(clicked()),
                this, SIGNAL(canceled()));


                QHBoxLayout *line1Layout = new QHBoxLayout;
                line1Layout->addWidget(nameLabel);
                line1Layout->addWidget(nameLineEdit);
    
                QHBoxLayout *line2Layout = new QHBoxLayout;
                line2Layout->addWidget(browseLabelOld);
                line2Layout->addWidget(browseLineEditOld);
                line2Layout->addWidget(browseButtonOld);
	
		QHBoxLayout *line3Layout = new QHBoxLayout;
                line3Layout->addWidget(browseLabelNew);
                line3Layout->addWidget(browseLineEditNew);
                line3Layout->addWidget(browseButtonNew);

                QHBoxLayout *line4Layout = new QHBoxLayout;
                line4Layout->addWidget(okButton);
                line4Layout->addWidget(cancelButton);

                QVBoxLayout *mainLayout = new QVBoxLayout;
                mainLayout->addLayout(line1Layout);
                mainLayout->addLayout(line2Layout);
                mainLayout->addLayout(line3Layout);
		mainLayout->addLayout(line4Layout);
                setLayout(mainLayout);

		m_parser = new Parser;
}

void DiffWidget::enableOkButton(const QString &text)
{
	Q_UNUSED(text);
        okButton->setEnabled(!browseLineEditOld->text().isEmpty() && !browseLineEditNew->text().isEmpty());
}

void DiffWidget::getOldPath()
{
        QString path_old;

        path_old = QFileDialog::getOpenFileName(
        this,
        "Choose the Old XML file to open",
        QString::null,
        QString::null);
        browseLineEditOld->setText(path_old);
}

void DiffWidget::getNewPath()
{
	QString path_new;

	path_new = QFileDialog::getOpenFileName(
	this,
	"Choose the Modified XML file to open",
	QString::null,
	QString::null);
	browseLineEditNew->setText(path_new);
}

void DiffWidget::applyButtonPressed()
{
	QString OldPath = browseLineEditOld->text();
	QString NewPath = browseLineEditNew->text();
	QString fileName = nameLineEdit->text();

	if(m_parser->Read_XML(OldPath, NewPath, fileName) > 0)
	{
		QMessageBox::information(this,"Task Completed",
		QString("The XML diff file of ") + browseLineEditOld->text() + " and " + browseLineEditNew->text() +
		 " has been successfully completed.\n The file has been stored as:  " + nameLineEdit->text() + ".");
	}
}
