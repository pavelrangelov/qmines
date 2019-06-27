#include <QtGui>

#include "resultdialog.h"
#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////////
ResultDialog::ResultDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags)
{
    setupUi(this);

    m_Parent = (MainWindow*)parent;
    m_Index = -1;
    m_ReadOnly = false;

    //table->setHorizontalHeaderLabels( QStringList() << tr("Name") << tr("Top Results") << tr("Time") );
    //table->setColumnWidth(0, 200);
    //table->setColumnWidth(1, 100);
    //table->horizontalHeader()->setStretchLastSection(true);

    int g = m_Parent->m_Settings.gameType;

    if( g < GAME_CUSTOM )
    {
        for(int i=0; i<TOP_RESULTS_COUNT; i++)
        {
            QTableWidgetItem *ti;

            ti = new QTableWidgetItem;
            table->setItem(i, 0, ti);

            ti = new QTableWidgetItem;
            table->setItem(i, 1, ti);

            ti = new QTableWidgetItem;
            table->setItem(i, 2, ti);

            if( m_Parent->m_Settings.top[g][i].time != "00:00:00" )
            {
                table->item(i,0)->setText(m_Parent->m_Settings.top[g][i].name);
                table->item(i,1)->setText(m_Parent->m_Settings.top[g][i].time);
                table->item(i,2)->setText(m_Parent->m_Settings.top[g][i].dtim);
            }
            else
            {
                table->item(i,0)->setText("");
                table->item(i,1)->setText("");
                table->item(i,2)->setText("");
            }

            table->item(i,0)->setFlags(0/*Qt::ItemIsEnabled|Qt::ItemIsSelectable*/);
            table->item(i,1)->setFlags(0/*Qt::ItemIsEnabled|Qt::ItemIsSelectable*/);
            table->item(i,2)->setFlags(0/*Qt::ItemIsEnabled|Qt::ItemIsSelectable*/);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ResultDialog::setData(int index)
{
    m_Index = index;

    int g = m_Parent->m_Settings.gameType;

    if( g < GAME_CUSTOM && m_Index != -1 )
    {
        QTableWidgetItem *ti;

        table->item(index,0)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
        table->setCurrentCell(index,0);
        ti = table->item(index,0);
        table->openPersistentEditor(ti);
        table->editItem(ti);
    }
}

///////////////////////////////////////////////////////////////////////////////
void ResultDialog::setReadOnly()
{
    m_ReadOnly = true;

    int g = m_Parent->m_Settings.gameType;

    if( g < GAME_CUSTOM )
    {
        for(int i=0; i<TOP_RESULTS_COUNT; i++)
        {
            table->item(i,0)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void ResultDialog::on_btnOK_clicked()
{
    if( !m_ReadOnly )
    {
        int g = m_Parent->m_Settings.gameType;

        if( g < GAME_CUSTOM && m_Index != -1 )
        {
            QTableWidgetItem *ti = table->item(m_Index, 0);
            table->closePersistentEditor(ti);

            if( table->item(m_Index,0)->text().isEmpty() )
            {
                table->item(m_Index,0)->setText(tr("Anonymous"));
            }

            for(int i=0; i<TOP_RESULTS_COUNT; i++)
            {
                m_Parent->m_Settings.top[g][i].name = table->item(i,0)->text();
                m_Parent->m_Settings.top[g][i].time = table->item(i,1)->text();
                m_Parent->m_Settings.top[g][i].dtim = table->item(i,2)->text();
            }

            m_Parent->saveSettings();
        }
    }

    accept();
}

///////////////////////////////////////////////////////////////////////////////
void ResultDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    int vhw = table->verticalHeader()->width() == 0 ? 26 : table->verticalHeader()->width();

    int w = table->width() - vhw - table->autoScrollMargin();

    table->setColumnWidth(0, w*0.4);
    table->setColumnWidth(1, w*0.2);
    table->setColumnWidth(2, w*0.4);

    QDialog::resizeEvent(event);
}
