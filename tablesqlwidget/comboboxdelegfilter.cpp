#include "comboboxdelegfilter.h"
#include <QDateEdit>

// Предоставление редактора
QWidget*  ComboBoxFileldDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option,
												 const QModelIndex & index ) const
{
	if (index.column()>2){

		// QMessageBox::critical(0,"",index.model()->data(index.sibling(index.row(),0)).toString());
		QString pole = index.model()->data(index.sibling(index.row(),0)).toString();
		int j;
		for(int i=0;i<fieldName.count();i++){
				QString s = model->headerData( fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n"," ");
				if (s==pole){
					j=fieldName.at(i);
					break;
				}
		}

		//QMessageBox::critical(0,"",QString("%1").arg(j));
		QSqlRelation rel =  model->relation(j);
		if (rel.indexColumn()!=QString("")){
			//QMessageBox::critical(0,"",rel.indexColumn()+" "+rel.displayColumn()+" "+rel.tableName());
			QComboBox * pRes = new QComboBox(parent);
			QSqlTableModel* relModel = new QSqlTableModel;

			relModel->setTable(rel.tableName());
			relModel->select();

			pRes->setModel(relModel);
			pRes->setModelColumn(relModel->fieldIndex(rel.displayColumn()));

			return pRes;

		}

		if (model->data(model->index(0,j), Qt::EditRole).type() == QVariant::Date){
			QDateEdit* pRes = new  QDateEdit(parent);
			pRes->setCalendarPopup(true);
			pRes->setDisplayFormat("dd.MM.yyyy");
			return pRes;
		}

		if (model->data(model->index(0,j), Qt::EditRole).type() == QVariant::Bool){
			QComboBox * pRes = new QComboBox(parent);

			pRes->addItem(tr("Нет"));
			pRes->addItem(tr("Да"));
			return pRes;
		}


		return QItemDelegate::createEditor(parent,option,index);
	}

	QComboBox * pRes = new QComboBox(parent);

	switch (index.column()) {
		case 0: {

			QStringList field;
			for(int i=0;i<fieldName.count();i++){
				QString s = model->headerData( fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n"," ");
				field<<s;
			}
			pRes->addItems(field);
			break;
		}
		case 1: {
			pRes->addItem(tr(" "));
			pRes->addItem(tr("не"));
			break;
		}
		case 2: {
			pRes->addItem(tr("равно"));
			pRes->addItem(tr("похоже на"));
			pRes->addItem(tr("больше"));
			pRes->addItem(tr("меньше"));
			break;
		}

	}

	// это строка нужна для того чтобы по enter и esc завершалось редактирование итд
	pRes->installEventFilter(const_cast<ComboBoxFileldDelegate*>(this));

	return pRes;

};

void ComboBoxFileldDelegate::setModel(ProfimaxRelationalModel *model){
	this->model = model;
}

// Делегат должен предоставить функцию копирования данных модели в редактор.
void ComboBoxFileldDelegate::setEditorData( QWidget * editor, const QModelIndex & index )const {

	QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
	if (pRes) {
		if (index.column()>2){

			QString pole = index.model()->data(index.sibling(index.row(),0)).toString();
			int j;
			for(int i=0;i<fieldName.count();i++){
				QString s = model->headerData( fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n"," ");
				if (s==pole){
					j=fieldName.at(i);
					break;
				}
			}

			if (model->data(model->index(0,j), Qt::EditRole).type() == QVariant::Bool){
					if (index.model()->data(index,Qt::EditRole).toInt()>0)
						pRes->setCurrentIndex(1);
					else
						pRes->setCurrentIndex(0);
					return;

			}
		}else{
			QString str = index.model()->data(index,Qt::EditRole).toString();
			int index = pRes->findText(str);
			if (index == -1)
				index = 0;
			pRes->setCurrentIndex(index);
		}
	}
	else
	{
		QItemDelegate::setEditorData(editor,index);
	}

};

// Запись данных в модель
void ComboBoxFileldDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const {


	QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
	if (pRes) {
		if (index.column()>2){

			QString pole = index.model()->data(index.sibling(index.row(),0)).toString();
			int j;
			for(int i=0;i<fieldName.count();i++){
				QString s = this->model->headerData( fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n"," ");
				if (s==pole){
					j=fieldName.at(i);
					break;
				}
			}

			if (this->model->data(this->model->index(0,j), Qt::EditRole).type() == QVariant::Bool){
				model->setData(index,pRes->currentIndex(),Qt::EditRole);
				return;
			}

			QString str = pRes->model()->data(pRes->model()->index(pRes->currentIndex(),0)).toString();
			model->setData(index,str,Qt::EditRole);

		}else{
			QString str = pRes->currentText();
			model->setData(index,str,Qt::EditRole);

			if (index.column()==0)
				model->setData(index.sibling(index.row(),3),"",Qt::EditRole);
		}
	}else{
		QItemDelegate::setModelData(editor,model,index);
	}
};

//Обновление геометрии редактора
void ComboBoxFileldDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
	editor->setGeometry(option.rect);
};

void ComboBoxFileldDelegate::setFieldName(QList<int> Name) {
	fieldName = Name;
};

void ComboBoxFileldDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	QString pole = index.model()->data(index.sibling(index.row(),0)).toString();
	int j;
	for(int i=0;i<fieldName.count();i++){
		QString s = model->headerData( fieldName.at(i) , Qt::Horizontal).toString();
		s.replace("\n"," ");
		if (s==pole){
			j=fieldName.at(i);
			break;
		}
	}

	QSqlRelation rel =  model->relation(j);
	if (index.column()>2){
		if (rel.indexColumn()!=QString("")){
			QSqlQuery query;
			query.exec(QString("SELECT * FROM %1 WHERE %2='%3'").arg(rel.tableName()).arg(rel.indexColumn()).arg(index.model()->data(index).toString()));
			while (query.next())
				drawDisplay(painter, option, option.rect,query.record().value(rel.displayColumn()).toString());
			return;
		}

		if (model->data(model->index(0,j), Qt::EditRole).type() == QVariant::Date){
			drawDisplay(painter, option, option.rect, index.model()->data(index, Qt::EditRole).toDate().toString("dd.MM.yyyy"));
			return;
		}

		if (this->model->data(this->model->index(0,j), Qt::EditRole).type() == QVariant::Bool){
			if (index.model()->data(index, Qt::DisplayRole).toInt()>0)
				drawDisplay(painter, option, option.rect, tr("Да"));
			else
				drawDisplay(painter, option, option.rect, tr("Нет"));
			drawFocus(painter, option, option.rect);
			return;
		}


	}


	QItemDelegate::paint(painter, option, index);
}



