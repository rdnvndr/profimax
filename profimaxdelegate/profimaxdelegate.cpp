#include "profimaxdelegate.h"
#include <QDateEdit>

// Предоставление редактора
QWidget*  ProfimaxDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option,
												 const QModelIndex & index ) const
{

	for (int j=0;j<fieldName.count();j++) {
		if (index.column()==fieldName.at(j).toInt()){
			QComboBox * pRes = new QComboBox(parent);

			pRes->addItem(tr("Нет"));
			pRes->addItem(tr("Да"));
			return pRes;
		}
	}

	if (index.model()->data(index, Qt::EditRole).type() == QVariant::Date){

		QDateEdit* pRes = new  QDateEdit(parent);
		pRes->setCalendarPopup(true);
		pRes->setDisplayFormat("dd.MM.yyyy");
		return pRes;
	}

	return QSqlRelationalDelegate::createEditor(parent,option,index);
};

// Делегат должен предоставить функцию копирования данных модели в редактор.
void ProfimaxDelegate::setEditorData( QWidget * editor, const QModelIndex & index )const {
	for (int j=0;j<fieldName.count();j++) {
		if (index.column()==fieldName.at(j).toInt()){
			QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
			if (pRes) {
				if (index.model()->data(index,Qt::EditRole).toInt()>0)
					pRes->setCurrentIndex(1);
				else
					pRes->setCurrentIndex(0);
				return;
			}

		}
	}

	if (index.model()->data(index, Qt::EditRole).type() == QVariant::Date){
		QDateEdit *pRes = static_cast<QDateEdit*>(editor);
		pRes->setDate(index.model()->data(index, Qt::EditRole).toDate());
		return;
	}

	//QSqlRelationalDelegate::setEditorData(editor,index);
	QItemDelegate::setEditorData(editor, index);
	return;

};

// Запись данных в модель
void ProfimaxDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const {
	for (int j=0;j<fieldName.count();j++) {
		if (index.column()==fieldName.at(j).toInt()){
			QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
			if (pRes) {
				if (index,pRes->currentIndex()==0)
					model->setData(index,false,Qt::EditRole);
				else
					model->setData(index,true,Qt::EditRole);
				return;
			}
		}
	}

	if (index.model()->data(index, Qt::EditRole).type() == QVariant::Date){
		QDateEdit *pRes = static_cast<QDateEdit*>(editor);
		model->setData(index,pRes->date(),Qt::EditRole);
		return;
	}

	ProfimaxRelationalModel *sqlModel = dynamic_cast<ProfimaxRelationalModel *>(model);
	QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
	QComboBox *combo = qobject_cast<QComboBox *>(editor);
	if (sqlModel && childModel && combo) {
		int currentItem = combo->currentIndex();
		int childColIndex = childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn());
		int childEditIndex = childModel->fieldIndex(sqlModel->relation(index.column()).indexColumn());
		sqlModel->setData(index,
				childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole),
				Qt::DisplayRole);
		sqlModel->setData(index,
				childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole),
				Qt::EditRole);
		//QMessageBox::critical(0,"",childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole).toString());
		return;
	}

	QSqlRelationalDelegate::setModelData(editor,model,index);
	return;
};

//Обновление геометрии редактора
void ProfimaxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
	editor->setGeometry(option.rect);
};

void ProfimaxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	for (int j=0;j<fieldName.count();j++) {
		if (index.column()==fieldName.at(j).toInt()){
			if (index.model()->data(index, Qt::DisplayRole).toInt()>0)
				drawDisplay(painter, option, option.rect, tr("Да"));
			else
				drawDisplay(painter, option, option.rect, tr("Нет"));
			drawFocus(painter, option, option.rect);
			return;
		}
	}
	if (index.model()->data(index, Qt::EditRole).type() == QVariant::Date){
		drawDisplay(painter, option, option.rect, index.model()->data(index, Qt::EditRole).toDate().toString("dd.MM.yyyy"));
		return;
	}
	QSqlRelationalDelegate::paint(painter, option, index);
}

void ProfimaxDelegate::setFieldName(QStringList Name) {
	fieldName = Name;
};
