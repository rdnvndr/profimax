#include "tabwidgetmdi.h"
#include <QApplication>
#include <QTabBar>

TabWidgetMDI::TabWidgetMDI(QWidget *parent) :
    QTabWidget(parent)
{
}

void  TabWidgetMDI::mousePressEvent(QMouseEvent* pe){
	if (pe->button() == Qt::RightButton){
		m_ptDragPos=pe->pos();
	}
	QTabWidget::mousePressEvent(pe);

}

void  TabWidgetMDI::mouseMoveEvent(QMouseEvent* pe){

	if (pe->buttons() & Qt::RightButton){
		int distance = (pe->pos()-m_ptDragPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			startDrag();
	}
	QTabWidget::mouseMoveEvent(pe);
}

void  TabWidgetMDI::startDrag(){

	QTabBar* tabbar  = qobject_cast<QTabBar *>(this->childAt(m_ptDragPos));
	int tabindex;
	if (tabbar!=NULL){
	    QMimeData* mimeData = new QMimeData();
	    tabindex = tabbar->tabAt(m_ptDragPos);
	    mimeData->setText(this->widget(tabindex)->objectName());

	    QDrag* drag = new QDrag(this);
	    //drag->setPixmap(this->icon().pixmap(32,32,QIcon::Normal));
	    drag->setMimeData(mimeData);
	    drag->start(Qt::MoveAction);
	}
}
