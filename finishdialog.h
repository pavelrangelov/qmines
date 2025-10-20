#ifndef FINISHDIALOG_H
#define FINISHDIALOG_H

#include <QDialog>
#include "ui_finishdialog.h"

namespace Ui {
class FinishDialog;
}

//-----------------------------------------------------------------------------
class FinishDialog: public QDialog {
    Q_OBJECT

    private:
        Ui::FinishDialog *ui;

	public:
        FinishDialog(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
		void setMessage(QString message);
};

#endif
