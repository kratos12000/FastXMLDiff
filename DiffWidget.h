#ifndef DIFFWIDGET_H
#define DIFFWIDGET_H


#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class Parser;

class DiffWidget : public QDialog
{
        Q_OBJECT

        public:
                DiffWidget(QWidget *parent = 0);
                DiffWidget(QString fname1, QString fname2, QWidget *parent = 0);
                DiffWidget(QString fname1, QString fname2, QString output, QWidget *parent = 0);

        signals:
		void canceled();

	public slots:
		void applyButtonPressed();

        private slots:
                void getOldPath();
		void getNewPath();
                void enableOkButton(const QString &text);

        private:
                QLabel *nameLabel;
                QLineEdit *nameLineEdit;
                QLabel *browseLabelOld;
		QLabel *browseLabelNew;
                QLineEdit *browseLineEditOld;
		QLineEdit *browseLineEditNew;
                QPushButton *browseButtonOld;
		QPushButton *browseButtonNew;
                QPushButton *okButton;
		QPushButton *cancelButton;
		Parser*	m_parser;

		void init();
};

#endif
