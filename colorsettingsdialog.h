#ifndef COLORSETTINGSDIALOG_H
#define COLORSETTINGSDIALOG_H

#include <QDialog>
#include <QColor>
#include <QString>
#include <QTimer>
#include <QResizeEvent>

#include "ui_colorsettingsdialog.h"
#include "mainwindow.h"

namespace Ui {
class ColorSettingsDialog;
}

//-----------------------------------------------------------------------------
class ColorSettingsDialog: public QDialog, public Ui_ColorSettingsDialog {
		Q_OBJECT

	public:
        ColorSettingsDialog(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());

	private:
        Ui::ColorSettingsDialog *ui;
		MainWindow *m_Parent;
		QColor m_CC;
		QColor m_OC;

    public slots:
        void acceptChanges();
        void resetToDefault();
        void changeClosedColor();
        void changeOpenedColor();
};

#endif
