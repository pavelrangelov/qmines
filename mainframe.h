#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QFrame>
#include <QPaintEvent>
#include <QPixmap>
#include <QPen>
#include <QSound>

#include "mainwindow.h"

#define MAX_W   100
#define MAX_H   100

class QString;
class QFont;
class QStaticText;

class MainFrame: public QFrame {
		Q_OBJECT

	public:
		MainFrame(QWidget *parent = 0);
		~MainFrame();

		void setFieldSize(int width, int height);
		void generateMines(int num);
		void setGameEnable(bool enable);
		void showAllSquares();
		void showAllFlags();
		void pauseGame();
		void resumeGame();
		void setMarked(int x, int y);
		void setClosedColor(QColor color);
		void setOpenedColor(QColor color);
		void doResize();

	private:
		QPixmap m_PixmapMine;
		QPixmap m_PixmapFlag;
		QPixmap m_PixmapErrm;
		QPixmap m_PixmapQues;
		QPixmap m_ScaledMine;
		QPixmap m_ScaledFlag;
		QPixmap m_ScaledErrm;
		QPixmap m_ScaledQues;
		int m_NumX;
		int m_NumY;
		int m_SquareWidth;
		int m_FieldBegin;
		quint16 m_Field[MAX_W][MAX_H];
		quint16 m_MinesCount;
		quint16 m_FlagsCount;
		qint16 m_EmptySquares;
		bool m_GameFailed;
		bool m_GameEnabled;
		QColor m_ClosedColor;
		QColor m_OpenedColor;
		QColor m_PausedBgColor;
		QColor m_BlackColor;

		void drawSquare(QPainter *painter, quint16 mode, QRect rect);
		void processLeftButton(int x, int y);
		void processRightButton(int x, int y);
		void openSquares(int x, int y);
		void CLR_CLOSED(int x, int y);

		QPen m_Pen[9];
		QFont m_Font;
		QColor m_BorderLightColor;
		QColor m_BorderDarkColor;
		int m_BR;
		int m_RW_BR;
		int m_RH_BR;
		int m_PausedX;
		int m_PausedY;
		int m_PausedW;
		int m_PausedH;
		QFont m_PausedFont;

        QSound *m_click;
        QSound *m_explosion;
        QSound *m_flag;

		static const char *m_Text[9];

	protected:
		virtual void paintEvent(QPaintEvent*);
		virtual void mousePressEvent(QMouseEvent*);
		virtual void resizeEvent(QResizeEvent*);

		signals:
		void squarePressed(KEYMSG_t msg);
		void gameFailed();
		void gameDone();
		void flagsCountChanged(quint16 count);
};

#endif
