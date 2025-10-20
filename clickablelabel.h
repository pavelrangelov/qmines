#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

///////////////////////////////////////////////////////////////////////////////
class ClickableLabel: public QLabel {
		Q_OBJECT

	public:
        explicit ClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
        explicit ClickableLabel(const QString &text = "", QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
		~ClickableLabel();

	protected:
		void mousePressEvent(QMouseEvent *event);

    signals:
		void clicked();
};

#endif // CLICKABLELABEL_H
