#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QSplashScreen>
#include <QString>
#include <QColor>

#include "finishdialog.h"

#define ORG_NAME    "Nifelheim"
#define APP_NAME    "QMines"
#define APP_VERS    "1.10"

#define DEFAULT_OPENED_COLOR    QColor(190,190,180) // QColor(200,200,200)
#define DEFAULT_CLOSED_COLOR    QColor(220,220,230) // QColor(170,170,170)

///////////////////////////////////////////////////////////////////////////////
namespace Ui {
class MainWindow;
}

typedef struct {
		int button;
		int x;
		int y;
} KEYMSG_t;

#define GAME_EASY           0
#define GAME_MEDIUM         1
#define GAME_HARD           2
#define GAME_CUSTOM			3

#define TOP_RESULTS_COUNT   10

#define DT_FORMAT "dd-MM-yyyy hh:mm:ss"

///////////////////////////////////////////////////////////////////////////////
class MainWindow: public QMainWindow {
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		int loadSettings();
		void saveSettings();

		QColor m_OpenedColor;
		QColor m_ClosedColor;

	private:
		Ui::MainWindow *ui;
		QTimer *m_Timer;
		QTimer *m_TimerBtn;
		QTime m_ElapsedTime;
		bool m_GameFailed;
		bool m_GameDone;
		bool m_GamePaused;
		bool m_TimerRunning;
		quint16 m_FlagsCount;
		quint16 m_MinesCount;
		quint16 m_NumX;
		quint16 m_NumY;
		int m_CurrGame;
		FinishDialog *m_FinishDialog;

		void startCounter();
		void stopCounter();
		void pauseCounter();
		int checkResult(QTime result);
		void showTopResult(int game);

		private slots:
		void on_action_NewGame_triggered();
		void on_action_PauseGame_triggered();
		void on_action_About_triggered();
		void on_action_Easy_triggered();
		void on_action_Medium_triggered();
		void on_action_Hard_triggered();
		void on_action_ColorSettings_triggered();
        void on_action_ShowToolbar_triggered();
        void on_action_EnableSounds_triggered();
		void on_action_TopResults_triggered();
		void on_btnNewGame_clicked();
		void on_toolNewGame_clicked();
		void on_toolPauseGame_clicked();
		void slot_timerBtnTout();
		void slot_squarePressed(KEYMSG_t msg);
		void slot_gameFailed();
		void slot_gameDone();
		void slot_tout1s();
		void slot_setFlagsCount(quint16 count);

		signals:
		void showFinishDialog();

	protected:
		virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
