#ifndef FINISHDIALOG_H
#define FINISHDIALOG_H

#include <QDialog>
#include "ui_finishdialog.h"

class FinishDialog: public QDialog, public Ui_FinishDialog {
		Q_OBJECT

	public:
		FinishDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		void setMessage(QString message);

		public slots:
		void on_btnOK_clicked();
};

#endif
