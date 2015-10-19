#include "welcometoolbutton.h"
#include <QContextMenuEvent>

WelcomeToolButton::WelcomeToolButton(QWidget * par)
{
	this->setParent(par);
	actionRemove = new QAction(tr("Убрать кнопку"),this);
	menu.addAction(actionRemove);
	connect(actionRemove,SIGNAL(triggered()),this,SIGNAL(removeButton()));
}

void WelcomeToolButton::contextMenuEvent(QContextMenuEvent *event)
{
	menu.exec(event->globalPos());
}

