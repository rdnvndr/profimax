#include "spravtoolbutton.h"
#include <QApplication>

SpravToolButton::SpravToolButton(QWidget * par)
{
	this->setParent(par);
}

void  SpravToolButton::mousePressEvent(QMouseEvent* pe){
	if (pe->button() == Qt::RightButton){
		m_ptDragPos=pe->pos();
	}
	QToolButton::mousePressEvent(pe);

}

void  SpravToolButton::mouseMoveEvent(QMouseEvent* pe){

	if (pe->buttons() & Qt::RightButton){
		int distance = (pe->pos()-m_ptDragPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			startDrag();
	}
	QToolButton::mouseMoveEvent(pe);
}

void  SpravToolButton::startDrag(){
	QMimeData* mimeData = new QMimeData();
	mimeData->setImageData(this->icon());
	//mimeData->setText(this->text ());
	mimeData->setText(this->objectName());
	//connect(toolButton, SIGNAL(clicked()), this, SLOT(callFuncButton()));

	QDrag* drag = new QDrag(this);
	drag->setPixmap(this->icon().pixmap(32,32,QIcon::Normal));
	drag->setMimeData(mimeData);
	drag->start(Qt::MoveAction);
}
