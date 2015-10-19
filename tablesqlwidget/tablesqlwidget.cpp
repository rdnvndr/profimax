/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "tablesqlwidget.h"
#include "comboboxdelegfilter.h"
#include "profimaxdelegate.h"
#include <QMessageBox>

TableSqlWidget::TableSqlWidget(QWidget* pwgt) : QWidget(pwgt) {
	setupUi(this);
	frameFilter->hide();
	model = new ProfimaxRelationalModel();

	connect(toolButtonInsert, SIGNAL(clicked()), this, SLOT(onClickToolButtonInsert()));
	connect(toolButtonDelete, SIGNAL(clicked()), this, SLOT(onClickToolButtonDelete()));
	connect(toolButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickToolButtonRefresh()));

	connect(toolButtonFilter, SIGNAL(clicked()), this, SLOT(onClickToolButtonFilter()));
	connect(toolButtonFilterApply, SIGNAL(clicked()), this, SLOT(onClickToolButtonFilterApply()));
	connect(toolButtonFilterCancel, SIGNAL(clicked()), this, SLOT(onClickToolButtonFilterCancel()));
	connect(toolButtonFilterAdd, SIGNAL(clicked()), this, SLOT(onClickToolButtonFilterAdd()));
	connect(toolButtonFilterRemove, SIGNAL(clicked()), this, SLOT(onClickToolButtonFilterRemove()));

	amodel= new QStandardItemModel(4, 4);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			QStandardItem *item = new QStandardItem(QString(""));
			amodel->setItem(row, column, item);
		}
	}
	tableViewFilter->setModel(amodel);

	amodel->setHeaderData(0, Qt::Horizontal, tr("Наименование столбца"));
	amodel->setHeaderData(1, Qt::Horizontal, tr("Отр."));
	amodel->setHeaderData(2, Qt::Horizontal, tr("Условие"));
	amodel->setHeaderData(3, Qt::Horizontal, tr("Значение"));
	tableViewFilter->setColumnWidth(0,200);
	tableViewFilter->setColumnWidth(1,40);
	tableViewFilter->setColumnWidth(2,70);

}


void TableSqlWidget::setEditStrategy ( QSqlTableModel::EditStrategy strategy ){
	//model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setEditStrategy(strategy);

}

// Добавление строки фильтра
void TableSqlWidget::onClickToolButtonFilterAdd() {

	int row = tableViewFilter->currentIndex().row();

	if (row<0)
		row=0;

	if (amodel->insertRow(row)) {

		for (int column = 0; column < 4; ++column) {
			QStandardItem *item = new QStandardItem(QString(""));
			amodel->setItem(row, column, item);
		}

		amodel->submit();
	}

}

// Удаление строки фильтра
void TableSqlWidget::onClickToolButtonFilterRemove() {
	int row = tableViewFilter->currentIndex().row();
	if (row<0)
		QMessageBox::critical(0,"",tr("Не выбрана строка"));
	else if (amodel->removeRow(row))
		amodel->submit();
}

// Применение фильтра
void TableSqlWidget::onClickToolButtonFilterApply() {

	QString filter = tr("");
	bool flag = false;
	for (int i=0;i<amodel->rowCount();i++) {

		QString fieldname = amodel->item(i,0)->text();
		if (fieldname!=QString("")) {
			if (flag)
				filter = filter+tr(" and ");
			else
				flag = true;

			for (int j=0;j<columnshow.count();j++) {
				QString s = model->headerData(model->fieldIndex(columnshow.at(j)), Qt::Horizontal).toString();
				//QString s=columnshow.at(j);
				s.replace("\n"," ");
				if (s==fieldname) {
					/*QSqlRelation rel =  model->relation(model->fieldIndex(columnshow.at(j)));
					if (rel.indexColumn()!=QString(""))
						fieldname= model->tableName()+QString(".")+rel.indexColumn();
					else*/
						fieldname= model->tableName()+QString(".")+columnshow.at(j);

					break;
				}
			}

			QString notif = amodel->item(i,1)->text();
			if (notif==tr("не"))
				notif="not";

			QString nameif = amodel->item(i,2)->text();
			if (nameif==tr("больше"))
				nameif=">";
			else if (nameif==tr("меньше"))
				nameif="<";
			else if (nameif==tr("равно"))
				nameif="=";
			else if (nameif==tr("похоже на"))
				nameif="like";
			else{
				QMessageBox::critical(0,tr("Ошибка фильтра"),tr("Условие не задано"));
				return;
			}

			QString valname = amodel->item(i,3)->text();
			if (valname ==tr("")){
				QMessageBox::critical(0,tr("Ошибка фильтра"),tr("Не задано значение"));
				return;
			}
			if (nameif==tr("like"))
				filter = filter+QString("  %1 %0 %2 '%%3%'").arg(fieldname).arg(notif).arg(nameif).arg(valname);
			else
				filter = filter+" "+notif+" "+fieldname+" "+nameif+" '"+valname+"'";
		}
	}

	if (filter == QString(""))
		return;

	if (this->filter == QString(""))
		model->setFilter(filter);
	else
		model->setFilter(QString("(%1) and (%2)").arg(this->filter).arg(filter));
	model->select();
}

// Сброс фильтра
void TableSqlWidget::onClickToolButtonFilterCancel() {
	model->setFilter(this->filter);
	model->select();
}

void TableSqlWidget::onClickToolButtonRefresh(){


	for (int i=0;i<model->columnCount();i++)
	    if (model->relation(i).isValid()){
		model->relationModel(i)->select();
		model->setRelation(i,model->relation(i));

	    }
	model->select();
/*
	// Обновление -новый вариант, но как обновить в диалоговых окнах ?
	QList<QSqlRelation> relation;
	QList<QVariant> header;
	QString filter = model->filter();

	for (int i=0;i<model->columnCount();i++){
		relation << model->relation(i);
		header << model->headerData(i, Qt::Horizontal);
	 }

	model->setTable(model->tableName());
	for (int i=0;i<model->columnCount();i++){
	    if (relation.at(i).isValid())
		model->setRelation(i,relation.at(i));
	    model->setHeaderData(i, Qt::Horizontal, header.at(i));
	}

	model->setFilter(filter);
	model->select();
*/

}

// Вставка строки
int TableSqlWidget::onClickToolButtonInsert(){


	QModelIndex mindex = tableViewSql->currentIndex();
	int row = mindex.row();

	if (row<0)
		row=0;

	QSqlRecord rec = model->record();
	for (int i=0;i<insrec.count();i=i+2){
		rec.setValue(insrec.at(i), insrec.at(i+1));
	}

	if (model->insertRecord(row,rec)) {
		model->submit();

		return row;
	}
	else{

		return -1;
	}

}

void TableSqlWidget::onClickToolButtonDelete() {
	int row = tableViewSql->currentIndex().row();
	if (row<0)
		QMessageBox::critical(0,"",tr("Не выбрана строка"));
	else
		if (model->removeRow(row))
			model->submit();
}

// Показать фильтр
void TableSqlWidget::onClickToolButtonFilter() {

	if (toolButtonFilter->isChecked()==true) {
		// Создаем список полей
		QList<int> headName;

		for (int i=0;i<columnshow.count();i++) {
			headName <<model->fieldIndex(columnshow.at(i));
		}

		ComboBoxFileldDelegate* combodeleg = new ComboBoxFileldDelegate();
		combodeleg->setFieldName(headName);
		combodeleg->setModel(model);

		tableViewFilter->setItemDelegate(combodeleg);

		frameFilter->show();
	} else
		frameFilter->hide();
}

void TableSqlWidget::setTable(const QString & tableName) {
	model->setTable(tableName);
	//model->select();
	tableViewSql->setModel(model);
	//tableViewSql->setItemDelegate(new QSqlRelationalDelegate(tableViewSql));
	//tableViewSql->setItemDelegate(new MySqlRelationalDelegate());
}

void TableSqlWidget::setHeader(const QString & col, const QString & colname) {
	model->setHeaderData(model->fieldIndex(col), Qt::Horizontal, colname);
}

void TableSqlWidget::removeColumn(const QString & col) {
	model->removeColumn(model->fieldIndex(col));
}

// Устанавливает видимы столбцы и их последовательность
void TableSqlWidget::setSortColumn(QStringList stl) {
	columnshow = stl;
	int tcount = tableViewSql->horizontalHeader()->count();

	for (int j=0;j<tcount;j++)
		tableViewSql->horizontalHeader()->setSectionHidden(tableViewSql->horizontalHeader()->visualIndex(j),true);

	for (int i=0;i<stl.count();i++)
		tableViewSql->horizontalHeader()->setSectionHidden(tableViewSql->horizontalHeader()->visualIndex(model->fieldIndex(stl.at(i))),false);

	tcount = tableViewSql->horizontalHeader()->count();
	int ind[tcount];
	for (int i=0;i<tcount;i++) {
		ind[i] = i;
	}

	for (int i=0;i<stl.count();i++)
		for (int j=i+1;j<tcount;j++)
			if (ind[j]==model->fieldIndex(stl.at(i))) {
				tableViewSql->horizontalHeader()->swapSections(i, j);
				int tmp = ind[i];
				ind[i]=ind[i];
				ind[j]=tmp;
			}
}

void TableSqlWidget::show() {
	tableViewSql->show();
}

void TableSqlWidget::select() {
	model->select();
	tableViewSql->setModel(model);
	//tableViewSql->setItemDelegate(new QSqlRelationalDelegate(tableViewSql));
	ProfimaxDelegate* mysqlrel= new ProfimaxDelegate();

	QStringList field;
	for (int j=0;j<fieldNameBool.count();j++)
		field <<  QString("%1").arg(model->fieldIndex(fieldNameBool.at(j)));

	mysqlrel->setFieldName(field);
	tableViewSql->setItemDelegate(mysqlrel);

}

void TableSqlWidget::setFieldNameBool(QStringList Name) {
	fieldNameBool = Name;
};

void TableSqlWidget::setButtoIconSize(int size){
	QSize sz = QSize(size,size);
	toolButtonDelete->setIconSize(sz);
	toolButtonEdit->setIconSize(sz);
	toolButtonFilter->setIconSize(sz);
	toolButtonFirst->setIconSize(sz);
	toolButtonInsert->setIconSize(sz);
	toolButtonLast->setIconSize(sz);
	toolButtonRefresh->setIconSize(sz);
}

void TableSqlWidget::setMargin(int size){
	verticalLayoutMain->setMargin(size);
	verticalLayoutMain->setSpacing(2);

}

void TableSqlWidget::setInsertValue(QStringList Name){
	insrec = Name;
}

void TableSqlWidget::setFilter(QString value){
	filter = value;
	model->setFilter(filter);
}

QVariant TableSqlWidget::getCurrentValue(QString s){
	int mapidx = tableViewSql->currentIndex().row();
	if (mapidx==-1)
			mapidx=0;

	return model->record(mapidx).value(s);
}

void TableSqlWidget::setCurrentValue(QString s, QVariant v){
	int mapidx = tableViewSql->currentIndex().row();
	if (mapidx==-1)
			mapidx=0;

	return model->record(mapidx).setValue(s,v);
}

// Установить связь поля с полем другой таблицы
void TableSqlWidget::setRelation(QString mycol,QString table,QString linkcol,QString showcol){
	model->setRelation(model->fieldIndex(mycol),  QSqlRelation(table, linkcol, showcol));
}

void TableSqlWidget::disconnectButtonInsert(){

    disconnect(toolButtonInsert, SIGNAL(clicked()), this, SLOT(onClickToolButtonInsert()));

}

void TableSqlWidget::disconnectButtonDelete(){

    disconnect(toolButtonDelete, SIGNAL(clicked()), this, SLOT(onClickToolButtonDelete()));

}

void TableSqlWidget::disconnectButtonRefresh(){

    disconnect(toolButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickToolButtonRefresh()));

}
