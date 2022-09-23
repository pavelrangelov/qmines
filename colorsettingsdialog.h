#ifndef COLORSETTINGSDIALOG_H
#define COLORSETTINGSDIALOG_H

#include <QDialog>
#include <QColor>
#include <QString>
#include <QTimer>
#include <QResizeEvent>

#include "ui_colorsettingsdialog.h"
#include "mainwindow.h"
#include "clickablelabel.h"

class ColorSettingsDialog: public QDialog, public Ui_ColorSettingsDialog {
		Q_OBJECT

	public:
		ColorSettingsDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	private:
		MainWindow *m_Parent;
		QColor m_CC;
		QColor m_OC;

		public slots:
		void on_btnOK_clicked();
		void on_btnCancel_clicked();
		void on_btnReset_clicked();
		void slot_labelClosedColor_clicked();
		void slot_labelOpenedColor_clicked();
};

#endif
