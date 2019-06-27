#include <QtGui>
#include <time.h>

#include "mainframe.h"

#define MODE_IS_CLOSED      0x0100
#define MODE_HAS_FLAG       0x0200
#define MODE_HAS_QUEST      0x0400
#define MODE_IS_PAUSED		0x0800
#define MODE_IS_MARKED      0x1000
#define MODE_ERR_MINE       0x2000
#define MODE_HAS_MINE       0x8000

#define HAS_MINE(x,y)       (m_Field[x][y] &   MODE_HAS_MINE)
#define SET_MINE(x,y)       (m_Field[x][y] |=  MODE_HAS_MINE)

#define HAS_FLAG(x,y)       (m_Field[x][y] &   MODE_HAS_FLAG)
#define SET_FLAG(x,y)       (m_Field[x][y] |=  MODE_HAS_FLAG)
#define CLR_FLAG(x,y)       (m_Field[x][y] &= ~MODE_HAS_FLAG)

#define HAS_QUEST(x,y)       (m_Field[x][y] &   MODE_HAS_QUEST)
#define SET_QUEST(x,y)       (m_Field[x][y] |=  MODE_HAS_QUEST)
#define CLR_QUEST(x,y)       (m_Field[x][y] &= ~MODE_HAS_QUEST)

#define IS_CLOSED(x,y)      (m_Field[x][y] &   MODE_IS_CLOSED)

#define SET_PAUSED(x,y)     (m_Field[x][y] |=  MODE_IS_PAUSED)
#define CLR_PAUSED(x,y)     (m_Field[x][y] &= ~MODE_IS_PAUSED)

#define SET_MARKED(x,y)     (m_Field[x][y] |=  MODE_IS_MARKED)
#define IS_MARKED(x,y)      (m_Field[x][y] &   MODE_IS_MARKED)

#define SET_ERRMINE(x,y)    (m_Field[x][y] |=  MODE_ERR_MINE)

#define _NW(x,y,maxx,maxy)	((x)>0&&(y)>0)
#define _N( x,y,maxx,maxy)	((y)>0)
#define _NE(x,y,maxx,maxy)	((x)<maxx&&(y)>0)
#define _W( x,y,maxx,maxy)	((x)>0)
#define _E( x,y,maxx,maxy)	((x)<maxx)
#define _SW(x,y,maxx,maxy)	((x)>0&&(y)<maxy)
#define _S( x,y,maxx,maxy)	((y)<maxy)
#define	_SE(x,y,maxx,maxy)	((x)<maxx&&(y)<maxy)

const char *MainFrame::m_Text[9] = {"1","2","3","4","5","6","7","8","?"};

///////////////////////////////////////////////////////////////////////////////
MainFrame::MainFrame(QWidget *parent)
{
    Q_UNUSED(parent);

    m_PixmapMine.load(":/images/mine.png");
    m_PixmapFlag.load(":/images/flag.png");
    m_PixmapErrm.load(":/images/nomine.png");
    m_PixmapQues.load(":/images/ques-64.png");

    m_NumX          = 10;
    m_NumY          = 10;
    m_MinesCount    = 10;

    m_Pen[0].setColor(QColor(0,     0,      255));  // 1 - blue
    m_Pen[1].setColor(QColor(0,     128,    0));    // 2 - green
    m_Pen[2].setColor(QColor(128,   128,    0));    // 3 - yellow
    m_Pen[3].setColor(QColor(128,   0,      128));  // 4 - purple
    m_Pen[4].setColor(QColor(255,   0,      0));    // 5 - red
    m_Pen[5].setColor(QColor(204,   204,    0));    // 6
    m_Pen[6].setColor(QColor(179,   102,    255));  // 7
    m_Pen[7].setColor(QColor(255,   153,    51));   // 8
    m_Pen[8].setColor(QColor(0,     0,      0));    // 9

    m_BorderLightColor  = QColor(250,250,250);
    m_BorderDarkColor   = QColor(120,120,120);

    m_PausedBgColor     = QColor(220,220,220);
    m_BlackColor        = QColor(0,0,0);

    #ifdef WIN32
    m_Font          .setFamily("Verdana");
    m_PausedFont    .setFamily("Verdana");
    #else
    m_Font          .setFamily("Ubuntu");
    m_PausedFont    .setFamily("Ubuntu");
    #endif
    m_Font.setBold(true);
}

///////////////////////////////////////////////////////////////////////////////
MainFrame::~MainFrame()
{
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::setFieldSize(int width, int height)
{
    m_NumX = width;
    m_NumY = height;
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re);

    doResize();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::paintEvent(QPaintEvent *pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);

    painter.setFont(m_Font);

    for(int x=0; x<m_NumX; x++)
    {
        for(int y=0; y<m_NumY; y++)
        {
            drawSquare(&painter, m_Field[x][y], QRect(m_FieldBegin+x*m_SquareWidth, y*m_SquareWidth, m_SquareWidth, m_SquareWidth));
        }
    }

    if( m_Field[0][0] & MODE_IS_PAUSED )
    {
        #define PAUSED_TEXT_MARGIN 10

        painter.setBrush(m_PausedBgColor);
        painter.setPen(m_BlackColor);
        painter.drawRect(m_PausedX,m_PausedY,m_PausedW,m_PausedH);
        painter.setFont(m_PausedFont);
        painter.drawText(
            QRect(m_PausedX+PAUSED_TEXT_MARGIN,m_PausedY+PAUSED_TEXT_MARGIN,
                  m_PausedW-2*PAUSED_TEXT_MARGIN,m_PausedH-2*PAUSED_TEXT_MARGIN),
            Qt::AlignCenter,
            "PAUSED");
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::mousePressEvent (QMouseEvent *me)
{
    QPointF pos = me->localPos();
    int button = me->button();

    KEYMSG_t msg;

    if( (pos.x() <= m_FieldBegin) ||
        (pos.x() >= m_FieldBegin+m_SquareWidth*m_NumX) ||
        (pos.y() <= 0) ||
        (pos.y() >= m_SquareWidth*m_NumY) )
    {
        return;
    }

    msg.x = (int)((pos.x() - m_FieldBegin) / m_SquareWidth);
    msg.y = (int)(pos.y() / m_SquareWidth);

    if( button == Qt::LeftButton )
    {
        processLeftButton(msg.x, msg.y);
        emit squarePressed(msg);
    }

    if( button == Qt::RightButton )
    {
        processRightButton(msg.x, msg.y);
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::doResize()
{
    QSize size = this->size();

    qreal width   = size.width()-1;
    qreal height  = size.height()-1;

    m_SquareWidth   = (int)( qMin(width/m_NumX, height/m_NumY) );
    m_FieldBegin    = (int)( (width - (m_NumX * m_SquareWidth))/2 );

    m_ScaledMine = m_PixmapMine.scaled(m_SquareWidth, m_SquareWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_ScaledFlag = m_PixmapFlag.scaled(m_SquareWidth, m_SquareWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_ScaledErrm = m_PixmapErrm.scaled(m_SquareWidth, m_SquareWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_ScaledQues = m_PixmapQues.scaled(m_SquareWidth, m_SquareWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_BR = m_SquareWidth / 25;

    if( m_BR < 2 )
    {
        m_BR = 2;
    }

    m_RW_BR = m_SquareWidth - m_BR;
    m_RH_BR = m_SquareWidth - m_BR;

    m_Font.setPixelSize(3 * qMin(m_SquareWidth,m_SquareWidth) / 4);

    QMainWindow *p = (QMainWindow*)this->parentWidget();

    int fgW = p->frameGeometry().width();
    int fgH = p->frameGeometry().height();

    m_PausedW = fgW / 6;
    m_PausedH = fgH / 6;
    m_PausedX = (fgW - m_PausedW) / 2;
    m_PausedY = (fgH - m_PausedH) / 2 - (fgH - this->height());
    m_PausedFont.setPixelSize(m_PausedW/10);
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::drawSquare(QPainter *painter, quint16 mode, QRect rect)
{
    int RX = rect.x();
    int RY = rect.y();
    int RW = rect.width();
    int RH = rect.height();

    painter->setPen(Qt::NoPen);

    // draw field
    if( mode & MODE_IS_CLOSED || mode & MODE_IS_PAUSED )
    {
        painter->setBrush(m_ClosedColor);
        painter->drawRect(rect);
    }
    else
    {
        if( mode & MODE_IS_MARKED )
        {
            painter->setBrush(QColor(200,0,0));
            painter->drawRect(rect);
        }
        else
        {
            painter->setBrush(m_OpenedColor);
            painter->drawRect(rect);
        }
    }

    // draw borders
    QPoint lborder[4], rborder[4], uborder[4], dborder[4];

    int RXoBR = RX + m_BR;
    int RYoBR = RY + m_BR;
    int RXoRW = RX + RW;
    int RYoRH = RY + RH;
    int RXoRW_BR = RX + m_RW_BR;
    int RYoRH_BR = RY + m_RH_BR;

    lborder[0] = QPoint(RX,			RY);
    lborder[1] = QPoint(RXoBR,		RYoBR);
    lborder[2] = QPoint(RXoBR,		RYoRH_BR);
    lborder[3] = QPoint(RX,			RYoRH);

    dborder[0] = QPoint(RX,			RYoRH);
    dborder[1] = QPoint(RXoBR,		RYoRH_BR);
    dborder[2] = QPoint(RXoRW_BR,	RYoRH_BR);
    dborder[3] = QPoint(RXoRW,		RYoRH);

    uborder[0] = QPoint(RX,         RY);
    uborder[1] = QPoint(RXoRW,      RY);
    uborder[2] = QPoint(RXoRW_BR,	RYoBR);
    uborder[3] = QPoint(RXoBR,		RYoBR);

    rborder[0] = QPoint(RXoRW,		RY);
    rborder[1] = QPoint(RXoRW,		RYoRH);
    rborder[2] = QPoint(RXoRW_BR,	RYoRH_BR);
    rborder[3] = QPoint(RXoRW_BR,	RYoBR);

    if( mode & MODE_IS_PAUSED )
    {
        painter->setBrush	(m_BorderDarkColor);
        painter->drawPolygon(rborder, 4);
        painter->drawPolygon(dborder, 4);

        painter->setBrush	(m_BorderLightColor);
        painter->drawPolygon(uborder, 4);
        painter->drawPolygon(lborder, 4);
    }
    else
    if( mode & MODE_IS_CLOSED )
    {
        painter->setBrush	(m_BorderDarkColor);
        painter->drawPolygon(rborder, 4);
        painter->drawPolygon(dborder, 4);

        painter->setBrush	(m_BorderLightColor);
        painter->drawPolygon(uborder, 4);
        painter->drawPolygon(lborder, 4);

        if( mode & MODE_HAS_FLAG )
        {
            painter->drawPixmap(RX, RY, m_ScaledFlag);
        }
        else
        if( mode & MODE_HAS_QUEST )
        {
            painter->setPen(m_Pen[8]);
            painter->drawText(QRect(RX,RY,RW,RH), Qt::AlignCenter, m_Text[8]);
        }
    }
    else
    {
        painter->setBrush	(m_BorderLightColor);
        painter->drawPolygon(rborder, 4);
        painter->drawPolygon(dborder, 4);

        painter->setBrush	(m_BorderDarkColor);
        painter->drawPolygon(uborder, 4);
        painter->drawPolygon(lborder, 4);

        if( mode & MODE_ERR_MINE )
        {
            painter->drawPixmap(RX, RY, m_ScaledErrm);
        }
        else
        if( mode & MODE_HAS_MINE )
        {
            painter->drawPixmap(RX, RY, m_ScaledMine);
        }
        else
        {
            mode &= 0x00ff;

            if( mode )
            {
                mode--;
                painter->setPen(m_Pen[mode]);
                painter->drawText(QRect(RX,RY,RW,RH), Qt::AlignCenter, m_Text[mode]);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::generateMines(int num)
{
    int i,x,y;

    m_MinesCount    = num;
    m_EmptySquares  = m_NumX * m_NumY - m_MinesCount;
    m_GameFailed    = false;
    m_FlagsCount    = 0;
    m_GameEnabled   = true;

    for(x=0; x<m_NumX; x++)
    {
        for(y=0; y<m_NumY; y++)
        {
            m_Field[x][y] = MODE_IS_CLOSED;
        }
    }

    qsrand(time(NULL));

    for(i=0; i<num; i++)
    {
        do
        {
            x = qrand()%m_NumX;
            y = qrand()%m_NumY;
        }
        while(HAS_MINE(x,y));

        SET_MINE(x,y);
    }

    for(x=0; x<m_NumX; x++)
    {
        for(y=0; y<m_NumY; y++)
        {
            if( HAS_MINE(x,y ) )
            {
                continue;
            }

            quint16 value = 0;
            int maxx = m_NumX - 1;
            int maxy = m_NumY - 1;

            if( _NW(x,y,maxx,maxy)	&& HAS_MINE(x-1,y-1) ) value++;
            if( _N (x,y,maxx,maxy)	&& HAS_MINE(x+0,y-1) ) value++;
            if( _NE(x,y,maxx,maxy)	&& HAS_MINE(x+1,y-1) ) value++;
            if( _W (x,y,maxx,maxy)	&& HAS_MINE(x-1,y+0) ) value++;
            if( _E (x,y,maxx,maxy)	&& HAS_MINE(x+1,y+0) ) value++;
            if( _SW(x,y,maxx,maxy)	&& HAS_MINE(x-1,y+1) ) value++;
            if( _S (x,y,maxx,maxy)	&& HAS_MINE(x+0,y+1) ) value++;
            if( _SE(x,y,maxx,maxy)	&& HAS_MINE(x+1,y+1) ) value++;

            m_Field[x][y] |= (value & 0x00ff);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::showAllSquares()
{
    for(int x=0; x<m_NumX; x++)
    {
        for(int y=0; y<m_NumY; y++)
        {
            CLR_CLOSED(x,y);

            if( HAS_FLAG(x,y) && !HAS_MINE(x,y) )
            {
                SET_ERRMINE(x,y);
            }
        }
    }
    update();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::showAllFlags()
{
    for(int x=0; x<m_NumX; x++)
    {
        for(int y=0; y<m_NumY; y++)
        {
            if( IS_CLOSED(x,y) && HAS_MINE(x,y) )
            {
                CLR_QUEST(x,y);
                SET_FLAG (x,y);
            }
            else
            {
                CLR_CLOSED(x,y);
            }
        }
    }
    update();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::processLeftButton(int x, int y)
{
    bool failed = false;

    if( !m_GameEnabled )
    {
        return;
    }

    if( HAS_FLAG(x,y) )
    {
        CLR_FLAG(x,y);
    }
    else
    if( HAS_QUEST(x,y) )
    {
        CLR_QUEST(x,y);
    }
    else
    {
        if( HAS_MINE(x,y) )
        {
            m_GameFailed = true;
            setMarked(x,y);
            showAllSquares();
            failed = true;
        }

        CLR_CLOSED(x,y);
        openSquares(x,y);
    }

    if( failed )
    {
        emit gameFailed();
    }

    update();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::processRightButton(int x, int y)
{
    if( !m_GameEnabled )
    {
        return;
    }

    if( IS_CLOSED(x,y) )
    {
        if( HAS_FLAG(x,y) )
        {
            CLR_FLAG (x,y);
            SET_QUEST(x,y);
            m_FlagsCount --;
        }
        else
        if( HAS_QUEST(x,y) )
        {
            CLR_QUEST(x,y);
        }
        else
        {
            SET_FLAG(x,y);
            m_FlagsCount ++;
        }

        update();
        emit flagsCountChanged(m_FlagsCount);
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::openSquares(int x, int y)
{
    int maxx = m_NumX - 1;
    int maxy = m_NumY - 1;

    if( x < 0 || x > maxx )
    {
        return;
    }
    if( y < 0 || y > maxy )
    {
        return;
    }
    if( _NW(x,y,maxx,maxy)	&& HAS_MINE(x-1,    y-1) ) return;
    if( _N (x,y,maxx,maxy)	&& HAS_MINE(x,      y-1) ) return;
    if( _NE(x,y,maxx,maxy)	&& HAS_MINE(x+1,    y-1) ) return;
    if( _W (x,y,maxx,maxy)	&& HAS_MINE(x-1,    y  ) ) return;
    if( _E (x,y,maxx,maxy)	&& HAS_MINE(x+1,    y  ) ) return;
    if( _SW(x,y,maxx,maxy)	&& HAS_MINE(x-1,    y+1) ) return;
    if( _S (x,y,maxx,maxy)	&& HAS_MINE(x,      y+1) ) return;
    if( _SE(x,y,maxx,maxy)	&& HAS_MINE(x+1,    y+1) ) return;

    if( _NW(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x-1,y-1) )
        {
            CLR_CLOSED  (x-1, y-1);
            openSquares (x-1, y-1);
        }
    }
    if( _N(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x+0,y-1) )
        {
            CLR_CLOSED  (x+0,y-1);
            openSquares (x+0,y-1);
        }
    }
    if( _NE(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x+1,y-1) )
        {
            CLR_CLOSED  (x+1,y-1);
            openSquares (x+1,y-1);
        }
    }
    if( _W(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x-1,y+0) )
        {
            CLR_CLOSED  (x-1,y+0);
            openSquares (x-1,y+0);
        }
    }
    if( _E(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x+1,y+0) )
        {
            CLR_CLOSED  (x+1,y+0);
            openSquares (x+1,y+0);
        }
    }

    if( _SW(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x-1,y+1) )
        {
            CLR_CLOSED  (x-1,y+1);
            openSquares (x-1,y+1);
        }
    }
    if( _S(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x+0,y+1) )
        {
            CLR_CLOSED  (x+0,y+1);
            openSquares (x+0,y+1);
        }
    }
    if( _SE(x,y,maxx,maxy) )
    {
        if( IS_CLOSED(x+1,y+1) )
        {
            CLR_CLOSED  (x+1,y+1);
            openSquares (x+1,y+1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::CLR_CLOSED(int x, int y)
{
    m_Field[x][y] &= ~MODE_IS_CLOSED;

    if( !m_GameFailed )
    {
        if( m_EmptySquares )
        {
            m_EmptySquares --;
        }

        if( m_EmptySquares <= 0 )
        {
            emit gameDone();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::setGameEnable(bool enable)
{
    m_GameEnabled = enable;
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::pauseGame()
{
    setGameEnable(false);

    for(int x=0; x<m_NumX; x++)
    {
        for(int y=0; y<m_NumY; y++)
        {
            SET_PAUSED(x,y);
        }
    }
    update();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::resumeGame()
{
    setGameEnable(true);

    for(int x=0; x<m_NumX; x++)
    {
        for(int y=0; y<m_NumY; y++)
        {
            CLR_PAUSED(x,y);
        }
    }
    update();
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::setMarked(int x, int y)
{
    SET_MARKED(x,y);
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::setClosedColor(QColor color)
{
    m_ClosedColor = color;
}

///////////////////////////////////////////////////////////////////////////////
void MainFrame::setOpenedColor(QColor color)
{
    m_OpenedColor = color;
}
