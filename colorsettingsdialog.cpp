#include <QColorDialog>

#include "colorsettingsdialog.h"
#include "mainwindow.h"
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
ColorSettingsDialog::ColorSettingsDialog(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::ColorSettingsDialog) {
    ui->setupUi(this);

	m_Parent = (MainWindow*) parent;
	m_CC = m_Parent->m_ClosedColor;
	m_OC = m_Parent->m_OpenedColor;

	QPixmap pixCC(48, 48);
	QPixmap pixOC(48, 48);
	pixCC.fill(m_Parent->m_ClosedColor);
	pixOC.fill(m_Parent->m_OpenedColor);

    ui->labelClosedColor->setPixmap(pixCC);
    ui->labelOpenedColor->setPixmap(pixOC);

    QObject::connect(ui->labelClosedColor, &ClickableLabel::clicked, this, &ColorSettingsDialog::changeClosedColor);
    QObject::connect(ui->labelOpenedColor, &ClickableLabel::clicked, this, &ColorSettingsDialog::changeOpenedColor);
    QObject::connect(ui->btnReset, &QPushButton::clicked, this, &ColorSettingsDialog::resetToDefault);
    QObject::connect(ui->btnOk, &QPushButton::clicked, this, &ColorSettingsDialog::acceptChanges);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, [this](){reject();});
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::acceptChanges() {
	m_Parent->m_ClosedColor = m_CC;
	m_Parent->m_OpenedColor = m_OC;

    g_Settings.closedColor = m_CC.rgb();
    g_Settings.openedColor = m_OC.rgb();

	m_Parent->saveSettings();
	accept();
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::resetToDefault() {
	m_CC = DEFAULT_CLOSED_COLOR;
	m_OC = DEFAULT_OPENED_COLOR;

	m_Parent->m_ClosedColor = m_CC;
	m_Parent->m_OpenedColor = m_OC;

	QPixmap pixCC(48, 48);
	QPixmap pixOC(48, 48);
	pixCC.fill(m_Parent->m_ClosedColor);
	pixOC.fill(m_Parent->m_OpenedColor);

    ui->labelClosedColor->setPixmap(pixCC);
    ui->labelOpenedColor->setPixmap(pixOC);
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::changeClosedColor() {
	QColorDialog dialog(this);
	m_CC = dialog.getColor(m_Parent->m_ClosedColor, this);

	if (m_CC.isValid()) {
		QPixmap pixCC(48, 48);
		pixCC.fill(m_CC);
        ui->labelClosedColor->setPixmap(pixCC);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ColorSettingsDialog::changeOpenedColor() {
	QColorDialog dialog(this);
	m_OC = dialog.getColor(m_Parent->m_OpenedColor, this);

	if (m_OC.isValid()) {
		QPixmap pixOC(48, 48);
		pixOC.fill(m_OC);
        ui->labelOpenedColor->setPixmap(pixOC);
	}
}
