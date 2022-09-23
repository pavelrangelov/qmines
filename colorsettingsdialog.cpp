#include <QColorDialog>

#include "colorsettingsdialog.h"
#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////////
ColorSettingsDialog::ColorSettingsDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags) {
	setupUi(this);

	m_Parent = (MainWindow*) parent;
	m_CC = m_Parent->m_ClosedColor;
	m_OC = m_Parent->m_OpenedColor;

	QPixmap pixCC(48, 48);
	QPixmap pixOC(48, 48);
	pixCC.fill(m_Parent->m_ClosedColor);
	pixOC.fill(m_Parent->m_OpenedColor);

	labelClosedColor->setPixmap(pixCC);
	labelOpenedColor->setPixmap(pixOC);

	connect(labelClosedColor, SIGNAL(clicked()), this, SLOT(slot_labelClosedColor_clicked()));
	connect(labelOpenedColor, SIGNAL(clicked()), this, SLOT(slot_labelOpenedColor_clicked()));
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::on_btnOK_clicked() {
	m_Parent->m_ClosedColor = m_CC;
	m_Parent->m_OpenedColor = m_OC;

	m_Parent->m_Settings.closedColor = m_CC.rgb();
	m_Parent->m_Settings.openedColor = m_OC.rgb();

	m_Parent->saveSettings();
	accept();
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::on_btnCancel_clicked() {
	reject();
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::on_btnReset_clicked() {
	m_CC = DEFAULT_CLOSED_COLOR;
	m_OC = DEFAULT_OPENED_COLOR;

	m_Parent->m_ClosedColor = m_CC;
	m_Parent->m_OpenedColor = m_OC;

	QPixmap pixCC(48, 48);
	QPixmap pixOC(48, 48);
	pixCC.fill(m_Parent->m_ClosedColor);
	pixOC.fill(m_Parent->m_OpenedColor);

	labelClosedColor->setPixmap(pixCC);
	labelOpenedColor->setPixmap(pixOC);
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::slot_labelClosedColor_clicked() {
	QColorDialog dialog(this);
	m_CC = dialog.getColor(m_Parent->m_ClosedColor, this);

	if (m_CC.isValid()) {
		QPixmap pixCC(48, 48);
		pixCC.fill(m_CC);
		labelClosedColor->setPixmap(pixCC);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::slot_labelOpenedColor_clicked() {
	QColorDialog dialog(this);
	m_OC = dialog.getColor(m_Parent->m_OpenedColor, this);

	if (m_OC.isValid()) {
		QPixmap pixOC(48, 48);
		pixOC.fill(m_OC);
		labelOpenedColor->setPixmap(pixOC);
	}
}
