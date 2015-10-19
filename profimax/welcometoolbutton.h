#ifndef WELCOMETOOLBUTTON_H
#define WELCOMETOOLBUTTON_H

#include <QToolButton>
#include <interfaces.h>
#include <QMenu>

class WelcomeToolButton : public QToolButton
{
	Q_OBJECT

public:
	QMenu menu;
	QAction* actionRemove;
	SpravInterface*	iSprav;

	WelcomeToolButton(QWidget * par = 0);

signals:
	 void removeButton();


protected:
	void contextMenuEvent(QContextMenuEvent *event);

};

#endif
