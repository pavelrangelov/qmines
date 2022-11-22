#include <QMessageBox>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finishdialog.h"
#include "resultdialog.h"
#include "colorsettingsdialog.h"
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	setWindowTitle (APP_NAME);
	setWindowIcon(QIcon(":/images/qmines.png"));

	ui->action_Custom->setEnabled(false);

	connect(ui->mainFrame, SIGNAL(squarePressed(KEYMSG_t)), this, SLOT(slot_squarePressed(KEYMSG_t)));
	connect(ui->mainFrame, SIGNAL(gameFailed()), this, SLOT(slot_gameFailed()));
	connect(ui->mainFrame, SIGNAL(gameDone()), this, SLOT(slot_gameDone()));
	connect(ui->mainFrame, SIGNAL(flagsCountChanged(quint16)), this, SLOT(slot_setFlagsCount(quint16)));

	m_TimerBtn = new QTimer(this);
	m_TimerBtn->setSingleShot(true);
	connect(m_TimerBtn, SIGNAL(timeout()), this, SLOT(slot_timerBtnTout()));

	QTimer *m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(slot_tout1s()));
	m_Timer->start(1000);

	m_ElapsedTime.setHMS(0, 0, 0, 0);

	m_TimerRunning = false;
	m_GameDone = false;
	m_GameFailed = false;
	m_GamePaused = false;

	m_FinishDialog = NULL;

    ui->action_ShowToolbar->setChecked(true);
    ui->action_EnableSounds->setChecked(false);

	on_action_NewGame_triggered();

	QSettings settings;
	restoreGeometry(settings.value(STORE_GEOMETRY).toByteArray());
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow() {
	delete ui;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_timerBtnTout() {
	if (!m_GameDone && !m_GameFailed) {
		ui->btnNewGame->setIcon(QIcon(":/images/happy-48.png"));
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_toolNewGame_clicked() {
	on_action_NewGame_triggered();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_NewGame_triggered() {
	if (m_FinishDialog) {
		if (m_FinishDialog->isVisible()) {
			m_FinishDialog->close();
		}
	}

	ui->action_Easy->setChecked(false);
	ui->action_Medium->setChecked(false);
	ui->action_Hard->setChecked(false);

	m_CurrGame = loadSettings();

	switch (m_CurrGame) {
		default:
		case GAME_EASY:
			m_NumX = 9;
			m_NumY = 9;
			m_MinesCount = 10;
			ui->action_Easy->setChecked(true);
			break;
		case GAME_MEDIUM:
			m_NumX = 16;
			m_NumY = 16;
			m_MinesCount = 40;
			ui->action_Medium->setChecked(true);
			break;
		case GAME_HARD:
			m_NumX = 30;
			m_NumY = 16;
			m_MinesCount = 99;
			ui->action_Hard->setChecked(true);
			break;
	}

	m_GameDone = false;
	m_GameFailed = false;
	m_GamePaused = false;
	m_TimerRunning = false;
	m_FlagsCount = 0;

	slot_setFlagsCount (m_FlagsCount);
	stopCounter();

	ui->btnNewGame->setIcon(QIcon(":/images/happy-48.png"));
	ui->mainFrame->setFieldSize(m_NumX, m_NumY);
	ui->mainFrame->generateMines(m_MinesCount);

	showTopResult (m_CurrGame);

	ui->valueMines->setText(QString("0/%1").arg(m_MinesCount));
	ui->valueTime->setText(QString("00:00:00"));

	ui->action_PauseGame->setEnabled(false);
	ui->toolPauseGame->setEnabled(false);

	ui->mainFrame->update();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_toolPauseGame_clicked() {
	on_action_PauseGame_triggered();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_PauseGame_triggered() {
	if (!m_GameDone && !m_GameFailed) {
		if (m_TimerRunning) {
			QString title = APP_NAME;
			title += " (";
			title += tr("Paused");
			title += ")";
			this->setWindowTitle(title);

			m_TimerRunning = false;
			m_GamePaused = true;
			ui->action_PauseGame->setIcon(QIcon(":/images/play-48.png"));
			ui->action_PauseGame->setText(tr("Resume Game"));
			ui->action_PauseGame->setToolTip(tr("Resume Game"));
			ui->toolPauseGame->setIcon(QIcon(":/images/play-48.png"));
			ui->mainFrame->pauseGame();
			ui->action_NewGame->setEnabled(false);
			ui->btnNewGame->setEnabled(false);
			ui->toolNewGame->setEnabled(false);
			ui->menu_Difficulty->setEnabled(false);
		} else {
			this->setWindowTitle(APP_NAME);

			m_TimerRunning = true;
			m_GamePaused = false;
			ui->action_PauseGame->setIcon(QIcon(":/images/pause-48.png"));
			ui->action_PauseGame->setText(tr("Pause Game"));
			ui->action_PauseGame->setToolTip(tr("Pause Game"));
			ui->toolPauseGame->setIcon(QIcon(":/images/pause-48.png"));
			ui->mainFrame->resumeGame();
			ui->action_NewGame->setEnabled(true);
			ui->btnNewGame->setEnabled(true);
			ui->toolNewGame->setEnabled(true);
			ui->menu_Difficulty->setEnabled(true);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event) {
	Q_UNUSED(event);
	saveSettings();
	event->accept();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_btnNewGame_clicked() {
	on_action_NewGame_triggered();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_Easy_triggered() {
	QSettings settings;
    g_Settings.gameType = GAME_EASY;
    settings.setValue(STORE_GAME_TYPE, g_Settings.gameType);
	on_btnNewGame_clicked();
	ui->mainFrame->doResize();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_Medium_triggered() {
	QSettings settings;
    g_Settings.gameType = GAME_MEDIUM;
    settings.setValue(STORE_GAME_TYPE, g_Settings.gameType);
	on_btnNewGame_clicked();
	ui->mainFrame->doResize();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_Hard_triggered() {
	QSettings settings;
    g_Settings.gameType = GAME_HARD;
    settings.setValue(STORE_GAME_TYPE, g_Settings.gameType);
	on_btnNewGame_clicked();
	ui->mainFrame->doResize();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_ColorSettings_triggered() {
	ColorSettingsDialog dialog(this);
	dialog.exec();
	on_action_NewGame_triggered();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_ShowToolbar_triggered() {
    QSettings settings;

    g_Settings.showToolbar = ui->action_ShowToolbar->isChecked();

    if (g_Settings.showToolbar) {
        ui->toolFrame->show();
    } else {
        ui->toolFrame->hide();
    }

    settings.setValue(STORE_SHOW_TOOLBAR, g_Settings.showToolbar);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_EnableSounds_triggered() {
    QSettings settings;
    g_Settings.enableSounds = ui->action_EnableSounds->isChecked();
    settings.setValue(STORE_ENABLE_SOUNDS, g_Settings.enableSounds);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_TopResults_triggered() {
	ResultDialog dialog(this);
	dialog.setReadOnly();
	dialog.exec();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::on_action_About_triggered() {
	QMessageBox::about(this, APP_NAME, tr("%1 - %2 by Nifelheim").arg(APP_NAME).arg(APP_VERS));
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_squarePressed(KEYMSG_t msg) {
	Q_UNUSED(msg);

	if (!m_GameDone && !m_GameFailed && !m_GamePaused) {
		m_TimerRunning = true;
		ui->btnNewGame->setIcon(QIcon(":/images/eh-48.png"));
		m_TimerBtn->start(200);

		if (!ui->action_PauseGame->isEnabled()) {
			ui->action_PauseGame->setEnabled(true);
			ui->toolPauseGame->setEnabled(true);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_gameFailed() {
	m_GameFailed = true;
	pauseCounter();
	ui->btnNewGame->setIcon(QIcon(":/images/sad-48.png"));
	ui->mainFrame->setGameEnable(false);

	m_FinishDialog = new FinishDialog(this, Qt::Dialog | Qt::CustomizeWindowHint);
	m_FinishDialog->setMessage("You have lost.");
	m_FinishDialog->show();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_gameDone() {
	if (!m_GameFailed && !m_GameDone) {
		m_GameDone = true;
		pauseCounter();
		ui->btnNewGame->setIcon(QIcon(":/images/laughter-48.png"));
		ui->valueMines->setText(QString("%1/%2").arg(m_MinesCount).arg(m_MinesCount));
		ui->mainFrame->setGameEnable(false);
		ui->mainFrame->showAllFlags();

		int index = checkResult(m_ElapsedTime);
		if (index != -1) {
			ResultDialog dialog(this);
			dialog.setData(index);
			dialog.exec();
			showTopResult (m_CurrGame);
		} else {
			m_FinishDialog = new FinishDialog(this, Qt::Dialog | Qt::CustomizeWindowHint);
			m_FinishDialog->setMessage("You have won!");
			m_FinishDialog->show();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_tout1s() {
	if (m_TimerRunning) {
		if (m_ElapsedTime.hour() < 23 && m_ElapsedTime.minute() < 59) {
			m_ElapsedTime = m_ElapsedTime.addSecs(1);
		} else {
			m_ElapsedTime.setHMS(23, 59, 59, 0);
		}

		QString text = m_ElapsedTime.toString("hh:mm:ss");
		ui->valueTime->setText(text);
	}
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::slot_setFlagsCount(quint16 count) {
	m_FlagsCount = count;
	ui->valueMines->setText(QString("%1/%2").arg(m_FlagsCount).arg(m_MinesCount));
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::startCounter() {
	m_TimerRunning = true;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::stopCounter() {
	m_TimerRunning = false;
	m_ElapsedTime.setHMS(0, 0, 0, 0);
	ui->valueTime->setText("00:00:00");
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::pauseCounter() {
	m_TimerRunning = false;
}

///////////////////////////////////////////////////////////////////////////////
int MainWindow::loadSettings() {
	QSettings settings;

    g_Settings.gameType = settings.value(STORE_GAME_TYPE, 0).toUInt();

    g_Settings.closedColor = settings.value(STORE_CLOSED_COLOR, DEFAULT_CLOSED_COLOR.rgb()).toUInt();
    g_Settings.openedColor = settings.value(STORE_OPENED_COLOR, DEFAULT_OPENED_COLOR.rgb()).toUInt();
    m_ClosedColor = QColor(g_Settings.closedColor);
    m_OpenedColor = QColor(g_Settings.openedColor);
	ui->mainFrame->setClosedColor(m_ClosedColor);
	ui->mainFrame->setOpenedColor(m_OpenedColor);

    g_Settings.showToolbar = settings.value(STORE_SHOW_TOOLBAR, true).toBool();
    ui->action_ShowToolbar->setChecked(g_Settings.showToolbar);

    g_Settings.enableSounds = settings.value(STORE_ENABLE_SOUNDS, true).toBool();
    ui->action_EnableSounds->setChecked(g_Settings.enableSounds);

    if (g_Settings.showToolbar) {
        ui->toolFrame->show();
    } else {
        ui->toolFrame->hide();
    }

    int g = g_Settings.gameType;

	if (g < GAME_CUSTOM) {
		// Create a list from 10 top result
        g_Settings.top[g].clear();
		for (int i = 0; i < TOP_RESULTS_COUNT; i++) {
			RESULT_t r;
			r.name = "";
			r.time = "00:00:00";
			r.dtim = "";
            g_Settings.top[g].append(r);
		}
		// Read saved list
		int size = settings.beginReadArray("Top");
		for (int i = 0; i < size; i++) {
			settings.setArrayIndex(i);
			switch (g) {
				case GAME_EASY:
                    g_Settings.top[g][i].name = settings.value("Easy/Name", "").toString();
                    g_Settings.top[g][i].time = settings.value("Easy/Time", "00:00:00").toString();
                    g_Settings.top[g][i].dtim = settings.value("Easy/DTim", "").toString();
					break;
				case GAME_MEDIUM:
                    g_Settings.top[g][i].name = settings.value("Medium/Name", "").toString();
                    g_Settings.top[g][i].time = settings.value("Medium/Time", "00:00:00").toString();
                    g_Settings.top[g][i].dtim = settings.value("Medium/DTim", "").toString();
					break;
				case GAME_HARD:
                    g_Settings.top[g][i].name = settings.value("Hard/Name", "").toString();
                    g_Settings.top[g][i].time = settings.value("Hard/Time", "00:00:00").toString();
                    g_Settings.top[g][i].dtim = settings.value("Hard/DTim", "").toString();
					break;
			}
		}
		settings.endArray();
	}

	return g;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::saveSettings() {
	QSettings settings;

    int g = g_Settings.gameType;

	settings.setValue(STORE_GEOMETRY, saveGeometry());

    settings.setValue(STORE_CLOSED_COLOR, g_Settings.closedColor);
    settings.setValue(STORE_OPENED_COLOR, g_Settings.openedColor);

    settings.setValue(STORE_SHOW_TOOLBAR, g_Settings.showToolbar);

	if (g < GAME_CUSTOM) {
		settings.beginWriteArray("Top");
		for (int i = 0; i < TOP_RESULTS_COUNT; i++) {
			settings.setArrayIndex(i);
			switch (g) {
				case GAME_EASY:
                    settings.setValue("Easy/Name", g_Settings.top[g][i].name);
                    settings.setValue("Easy/Time", g_Settings.top[g][i].time);
                    settings.setValue("Easy/DTim", g_Settings.top[g][i].dtim);
					break;
				case GAME_MEDIUM:
                    settings.setValue("Medium/Name", g_Settings.top[g][i].name);
                    settings.setValue("Medium/Time", g_Settings.top[g][i].time);
                    settings.setValue("Medium/DTim", g_Settings.top[g][i].dtim);
					break;
				case GAME_HARD:
                    settings.setValue("Hard/Name", g_Settings.top[g][i].name);
                    settings.setValue("Hard/Time", g_Settings.top[g][i].time);
                    settings.setValue("Hard/DTim", g_Settings.top[g][i].dtim);
					break;
			}
		}
		settings.endArray();
	}
}

///////////////////////////////////////////////////////////////////////////////
int MainWindow::checkResult(QTime result) {
	int i;
    int g = g_Settings.gameType;

	if (g < GAME_CUSTOM) {
		for (i = 0; i < TOP_RESULTS_COUNT; i++) {
            QTime time = QTime::fromString(g_Settings.top[g][i].time, "hh:mm:ss");

            if (result <= time || g_Settings.top[g][i].time == "00:00:00" || g_Settings.top[g][i].time.isEmpty()) {
				RESULT_t r;
				r.name = "";
				r.time = result.toString("hh:mm:ss");
				r.dtim = QDateTime::currentDateTime().toString(DT_FORMAT);
                g_Settings.top[g].insert(i, r);
                g_Settings.top[g].removeLast();
				return i;
			}
		}
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::showTopResult(int game) {
    if (game < GAME_CUSTOM && g_Settings.top[game][0].time != "00:00:00" && !g_Settings.top[game][0].time.isEmpty()) {
        ui->valueBest->setText(QString(g_Settings.top[game][0].time));
	} else {
		ui->valueBest->setText(QString("None"));
	}
}
