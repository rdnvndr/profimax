#ifndef SPRAVTOOLBUTTON_H
#define SPRAVTOOLBUTTON_H

#include <QToolButton>
#include <interfaces.h>
#include <QtGui>

class SpravToolButton : public QToolButton
{
	Q_OBJECT

public:
	SpravInterface*	iSprav;
	QPoint m_ptDragPos;

	SpravToolButton(QWidget * par = 0);

	virtual void mousePressEvent(QMouseEvent* pe);
	virtual void mouseMoveEvent(QMouseEvent* pe);
	void  startDrag();
};

#endif // SPRAVTOOLBUTTON_H
