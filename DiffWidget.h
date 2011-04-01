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

        signals:
		void canceled();

	public slots:
		void applyButtonPressed();
		void cancelButtonPressed();

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
};

#endif
