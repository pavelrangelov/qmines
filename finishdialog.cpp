#include <QObject>
#include <QToolButton>

#include "finishdialog.h"

///////////////////////////////////////////////////////////////////////////////
FinishDialog::FinishDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags), ui(new Ui::FinishDialog) {
    ui->setupUi(this);

    QObject::connect(ui->btnOk, &QToolButton::clicked, this, [this](){accept();});
}

///////////////////////////////////////////////////////////////////////////////
void FinishDialog::setMessage(QString message) {
    ui->btnOk->setText(message);
}
