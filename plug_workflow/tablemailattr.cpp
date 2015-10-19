#include "tablemailattr.h"

TableMailAttr::TableMailAttr(QWidget *parent) :
    QTableView(parent)
{
}

void TableMailAttr::keyPressEvent ( QKeyEvent * event ) {
    switch (event->key()) {

	//Удаление строки
	case Qt::Key_Delete:
	   // Проверка не пустая ли строка
	    if (this->model()->data(this->currentIndex().sibling(this->currentIndex().row(),0)).toString()!= tr(""))
		this->model()->removeRow(this->currentIndex().row());
	    break;

	default:
	    QTableView::keyPressEvent(event);
     }
}
