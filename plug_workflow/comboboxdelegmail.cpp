#include "comboboxdelegmail.h"
#include <QDateEdit>

// Предоставление редактора
QWidget*  ComboBoxMailDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{

	if (index.column() == 1) {

		// QMessageBox::critical(0,"",index.model()->data(index.sibling(index.row(),0)).toString());
		QString pole = index.model()->data(index.sibling(index.row(), 0)).toString();
		int j;
		for (int i = 0; i < fieldName.count(); i++) {
			QString s = model->headerData(fieldName.at(i) , Qt::Horizontal).toString();
			s.replace("\n", " ");

			if (s == pole) {
				j = fieldName.at(i);
				break;
			}
		}

		//QMessageBox::critical(0,"",QString("%1").arg(j));
		QSqlRelation rel =  model->relation(j);
		if (rel.indexColumn() != QString("")) {
			//QMessageBox::critical(0,"",rel.indexColumn()+" "+rel.displayColumn()+" "+rel.tableName());
			QComboBox* pRes = new QComboBox(parent);
			QSqlTableModel* relModel = new QSqlTableModel;
			relModel->setTable(rel.tableName());

			/* Удаление выбранных значений в кому */
			if (pole == tr("Кому")) {
				QString relFilter = QString("user_id != '00000000-0000-0000-0000-000000000000'");
				for (int i = 0; i < index.model()->rowCount(); i++) {

					QString pole = index.model()->data(index.sibling(i, 0)).toString();

					if (pole == tr("Кому") && index.row() != i) {
						QString val = index.model()->data(index.sibling(i, 1)).toString();
						if ( val != QString(""))
							relFilter = QString("%1 and not user_id = '%2'").arg(relFilter).arg(val);
					}
					//QMessageBox::critical(0,"",index.model()->data(index.sibling(i,1)).toString());
				}
				//QMessageBox::critical(0,"","-"+relFilter+"-");
				relModel->setFilter(relFilter);
			}

			relModel->select();

			pRes->setModel(relModel);
			pRes->setModelColumn(relModel->fieldIndex(rel.displayColumn()));

			return pRes;

		}

		if (model->data(model->index(0, j)).type() == QVariant::Date) {
			QDateEdit* pRes = new  QDateEdit(parent);
			pRes->setCalendarPopup(true);
			pRes->setDisplayFormat("dd.MM.yyyy");
			return pRes;
		}

		if (model->data(model->index(0, j)).type() == QVariant::Bool) {
			QComboBox* pRes = new QComboBox(parent);

			pRes->addItem(tr("Нет"));
			pRes->addItem(tr("Да"));
			return pRes;
		}


		return QItemDelegate::createEditor(parent, option, index);
	}

	if (index.column() == 0) {
		QComboBox* pRes = new QComboBox(parent);

		bool typeflag       = true;
		bool priorflag      = true;
		bool recipientflag  = true;
		bool beginflag  = true;
		bool endflag  = true;

		for (int i = 0; i < index.model()->rowCount(); i++) {

			QString pole = index.model()->data(index.sibling(i, 0)).toString();

			if (pole == tr("Тип") && index.row() != i)
				typeflag = false;
			if (pole == tr("Приоритет") && index.row() != i)
				priorflag = false;
			if (pole == tr("Начало") && index.row() != i)
				beginflag = false;
			if (pole == tr("Конец") && index.row() != i)
				endflag = false;
		}

		//if (recipientflag)
		pRes->addItem(tr("Кому"));

		if (typeflag)
			pRes->addItem(tr("Тип"));
		if (priorflag)
			pRes->addItem(tr("Приоритет"));
		if (beginflag)
			pRes->addItem(tr("Начало"));
		if (endflag)
			pRes->addItem(tr("Конец"));

		//pRes->addItem(tr("Копия"));

		// это строка нужна для того чтобы по enter и esc завершалось редактирование итд
		pRes->installEventFilter(const_cast<ComboBoxMailDelegate*>(this));

		return pRes;
	}


	return QItemDelegate::createEditor(parent, option, index);

};

void ComboBoxMailDelegate::setModel(ProfimaxRelationalModel* model)
{
	this->model = model;
}

// Делегат должен предоставить функцию копирования данных модели в редактор.
void ComboBoxMailDelegate::setEditorData(QWidget* editor, const QModelIndex& index)const
{

	QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
	if (pRes) {
		if (index.column() == 1) {

			QString pole = index.model()->data(index.sibling(index.row(), 0)).toString();
			int j;
			for (int i = 0; i < fieldName.count(); i++) {
				QString s = model->headerData(fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n", " ");
				if (s == pole) {
					j = fieldName.at(i);
					break;
				}
			}

			if (model->data(model->index(0, j), Qt::EditRole).type() == QVariant::Bool) {
				if (index.model()->data(index, Qt::EditRole).toInt() > 0)
					pRes->setCurrentIndex(1);
				else
					pRes->setCurrentIndex(0);
				return;

			}
		}
		else {
			QString str = index.model()->data(index, Qt::EditRole).toString();
			int index = pRes->findText(str);
			if (index == -1)
				index = 0;
			pRes->setCurrentIndex(index);

		}
	}
	else {
		QItemDelegate::setEditorData(editor, index);
	}

};

// Запись данных в модель
void ComboBoxMailDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)const
{


	QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
	if (pRes) {
		if (index.column() == 1) {

			QString pole = index.model()->data(index.sibling(index.row(), 0)).toString();
			int j;
			for (int i = 0; i < fieldName.count(); i++) {
				QString s = this->model->headerData(fieldName.at(i) , Qt::Horizontal).toString();
				s.replace("\n", " ");
				if (s == pole) {
					j = fieldName.at(i);
					break;
				}
			}

			if (this->model->data(this->model->index(0, j), Qt::EditRole).type() == QVariant::Bool) {
				model->setData(index, pRes->currentIndex(), Qt::EditRole);
				return;
			}

			QString str = pRes->model()->data(pRes->model()->index(pRes->currentIndex(), 0)).toString();
			model->setData(index, str, Qt::EditRole);

		}
		else {
			QString str = pRes->currentText();
			model->setData(index, str, Qt::EditRole);

			if (index.column() == 0)
				model->setData(index.sibling(index.row(), 1), "", Qt::EditRole);

			// Добавление строки
			bool spaceflag = false;
			for (int i = 0; i < index.model()->rowCount(); i++) {

				QString valstr = index.model()->data(index.sibling(i, 0)).toString();

				if (valstr == tr("")) {
					spaceflag = true;
					break;
				}
			}

			if (spaceflag == false) {

				int row = index.model()->rowCount();
				if (row < 0)
					row = 0;
				model->insertRow(row);
				model->submit();
			}
		}

	}
	else {
		QItemDelegate::setModelData(editor, model, index);
	}
};

//Обновление геометрии редактора
void ComboBoxMailDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
};

void ComboBoxMailDelegate::setFieldName(QList<int> Name)
{
	fieldName = Name;
};

void ComboBoxMailDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

	QString pole = index.model()->data(index.sibling(index.row(), 0)).toString();
	int j;
	for (int i = 0; i < fieldName.count(); i++) {
		QString s = model->headerData(fieldName.at(i) , Qt::Horizontal).toString();
		s.replace("\n", " ");
		if (s == pole) {
			j = fieldName.at(i);
			break;
		}
	}

	QSqlRelation rel =  model->relation(j);
	if (index.column() == 1) {
		if (rel.indexColumn() != QString("")) {
			QSqlQuery query;
			query.exec(QString("SELECT * FROM %1 WHERE %2='%3'").arg(rel.tableName()).arg(rel.indexColumn()).arg(index.model()->data(index).toString()));
			while (query.next())
				drawDisplay(painter, option, option.rect, query.record().value(rel.displayColumn()).toString());
			return;
		}

		if (model->data(model->index(0, j), Qt::EditRole).type() == QVariant::Date) {
			drawDisplay(painter, option, option.rect, index.model()->data(index, Qt::EditRole).toDate().toString("dd.MM.yyyy"));
			return;
		}

		if (this->model->data(this->model->index(0, j), Qt::EditRole).type() == QVariant::Bool) {
			if (index.model()->data(index, Qt::DisplayRole).toInt() > 0)
				drawDisplay(painter, option, option.rect, tr("Да"));
			else
				drawDisplay(painter, option, option.rect, tr("Нет"));
			drawFocus(painter, option, option.rect);
			return;
		}


	}


	QItemDelegate::paint(painter, option, index);
}



