#include "mytreewidget.h"
#include <QApplication>
#include <QDrag>
#include <QDropEvent>

MyTreeWidget::MyTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
     setAcceptDrops(true);



     //setDragEnabled(true);
     //setDragDropMode(QAbstractItemView::DragDrop);
}

bool MyTreeWidget::checkFormat(QDropEvent *ev)
{
    /*QStringList supportedFormats = GetFormat();
    foreach (QString format, supportedFormats)
    {
	if (ev->mimeData()->hasFormat(format))
	{ */
	   ev->acceptProposedAction();
	   return true;
	/*}
    }
    ev->ignore();
    return false; */
}

void  MyTreeWidget::mousePressEvent(QMouseEvent* pe){
	if (pe->button() == Qt::LeftButton){
		m_ptDragPos=pe->pos();
	}
	QTreeWidget::mousePressEvent(pe);

}

void  MyTreeWidget::mouseMoveEvent(QMouseEvent* pe){

	if (pe->buttons() & Qt::LeftButton){
		int distance = (pe->pos()-m_ptDragPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			startDrag();
	}
	QTreeWidget::mouseMoveEvent(pe);
}

void  MyTreeWidget::startDrag(){
    if (this->currentItem()==NULL)
	return;
    QMimeData* mimeData = new QMimeData();

    mimeData->setImageData(this->currentItem()->icon(0));
    mimeData->setText(this->objectName());

    QDrag* drag = new QDrag(this);
    drag->setPixmap(this->currentItem()->icon(0).pixmap(32,32,QIcon::Normal));
    drag->setMimeData(mimeData);
    drag->start(Qt::MoveAction);
}


QString MyTreeWidget::getPathFromItem(QTreeWidgetItem* item){
    QString itempath;
    while (item->parent() != NULL){
	itempath = QString("%1%2_").arg(itempath).arg(item->parent()->indexOfChild(item));
	item = item->parent();
    }

    itempath = QString("%1%2").arg(itempath).arg(this->indexOfTopLevelItem(item));
    return itempath;
}

QTreeWidgetItem* MyTreeWidget::getItemFromPath(QString list){
    QStringList slist = list.split("_");
    QTreeWidgetItem* item = this->topLevelItem(slist.at(slist.count()-1).toInt());

    for (int i=slist.count()-2;i>=0;i--)
	item = item->child((slist.at(i)).toInt());

    return item;
}

bool MyTreeWidget::searchShowItem(QString text,QTreeWidgetItem* item){
    bool flag = false;
    for (int i=0;i < item->childCount();i++){
	if (searchShowItem(text,item->child(i))||flag)
	    flag = true;
    }

    if (flag==false&&item->text(0).contains(text, Qt::CaseInsensitive))
	flag=true;

    if (flag==false)
	item->setHidden(true);
    else
	item->setHidden(false);
    return flag;
}

void MyTreeWidget::searchShowAllItem(QString text){

    for (int i=0;i<this->topLevelItemCount();i++)
	searchShowItem(text, this->topLevelItem(i));
}

void MyTreeWidget::showAllItem(){
    for (int i=0;i<this->topLevelItemCount();i++)
	this->topLevelItem(i)->setHidden(true);
}

void MyTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
	checkFormat(event);
}

void MyTreeWidget::dropEvent(QDropEvent *event){

    //Обработка событий
    if (iSprav!=NULL)
	if (iSprav->dragTreeWidget(event,this))
	    event->acceptProposedAction();
}

void MyTreeWidget::dragMoveEvent(QDragMoveEvent *ev)
{

    checkFormat(ev);
}

void MyTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // Вызов меню
   //  menu.exec(event->globalPos());
    emit contextMenuSignal(event);
}

/*void MyTreeWidget::contextMenuSignal()
{

}*/
