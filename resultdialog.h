#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QResizeEvent>

#include "ui_resultdialog.h"
#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////////
class ResultDialog : public QDialog, public Ui_ResultDialog
{
        Q_OBJECT

    public:
        ResultDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        void setData(int index);
        void setReadOnly();

    private:
        MainWindow *m_Parent;
        int m_Index;
        bool m_ReadOnly;

    protected:
        virtual void resizeEvent(QResizeEvent *event);

    public slots:
        void on_btnOK_clicked();
};

#endif
