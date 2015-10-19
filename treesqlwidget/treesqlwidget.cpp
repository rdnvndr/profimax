/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#include <QMessageBox>
#include "treesqlwidget.h"

TreeSqlWidget::TreeSqlWidget(QWidget* pwgt) : QWidget(pwgt) {
	setupUi(this);

	connect(toolButtonInsert, SIGNAL(clicked()), this, SLOT(onClickToolButtonInsert()));
	connect(toolButtonDelete, SIGNAL(clicked()), this, SLOT(onClickToolButtonDelete()));
	connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(onClickToolButtonEdit()));
	connect(toolButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickToolButtonRefresh()));
}

// Добавление строки фильтра
void TreeSqlWidget::onClickToolButtonFilterAdd() {

}

// Удаление строки фильтра
void TreeSqlWidget::onClickToolButtonFilterRemove() {

}

// Применение фильтра
void TreeSqlWidget::onClickToolButtonFilterApply() {

}

// Сброс фильтра
void TreeSqlWidget::onClickToolButtonFilterCancel() {

}

// Вставка строки
int TreeSqlWidget::onClickToolButtonInsert(){

}

void TreeSqlWidget::onClickToolButtonDelete() {

}

void TreeSqlWidget::onClickToolButtonEdit() {

}

void TreeSqlWidget::onClickToolButtonRefresh() {

	this->select();
}

// Показать фильтр
void TreeSqlWidget::onClickToolButtonFilter() {

}

void TreeSqlWidget::setTable(const QString & tableName) {
		this->tableName = tableName;
}

QString TreeSqlWidget::lineFields() {
	QString fields = QString("");
	int count=0;
	foreach (QString str,head){

	   fields = fields+str+",";
	   count++;
	}
	treeWidget->setColumnCount(count+1);
	fields = fields+id;
	return fields;
}

void TreeSqlWidget::setHeader(QStringList stl) {
	head = stl;
	treeWidget->setColumnCount(head.count());
}

void TreeSqlWidget::hideColumn(int col) {
	treeWidget->header()->hideSection(col);
}

void TreeSqlWidget::setNodeField(QString id,QString par){
	this->id = id;
	this->par = par;
}

void TreeSqlWidget::addNode(QTreeWidgetItem* widget,QString parent){

	QSqlQuery query;
	if (parent!=QString("NULL"))
		if (filter!=QString(""))
			query.exec(QString("SELECT %1 FROM %2 WHERE parent='%3' AND (%4)").arg(lineFields()).arg(tableName).arg(parent).arg(filter));
		else
			query.exec(QString("SELECT %1 FROM %2 WHERE parent='%3'").arg(lineFields()).arg(tableName).arg(parent));
	else
		if (filter!=QString(""))
			query.exec(QString("SELECT %1 FROM %2 WHERE parent IS NULL AND (%3)").arg(lineFields()).arg(tableName).arg(filter));
		else
			query.exec(QString("SELECT %1 FROM %2 WHERE parent IS NULL").arg(lineFields()).arg(tableName));

		int fieldID = query.record().indexOf(id);

		while (query.next()) {

			int i=0;
			QTreeWidgetItem *item = new QTreeWidgetItem(widget);
			foreach(QString str,head){
				QString name = query.value(query.record().indexOf(head.at(i))).toString();
				item->setText(i, name);
				i++;
			}
			QString ID = query.value(fieldID).toString();
			item->setText(i, ID);

			addNode(item,ID);
		}
}

void TreeSqlWidget::setHeaderLabels(QStringList headers){
	treeWidget->setHeaderLabels(headers);
}

void TreeSqlWidget::select() {
	treeWidget->clear();
	int i=0;
	QTreeWidgetItem *rootitem = new QTreeWidgetItem(treeWidget);
	treeWidget->setCurrentItem(rootitem);
	foreach(QString str,root){
		rootitem->setText(i, str);
		i++;
	}
	rootitem->setText(i, "NULL");
	addNode(rootitem,"NULL");


}

void TreeSqlWidget::setButtonIconSize(int size){
	QSize sz = QSize(size,size);
	toolButtonDelete->setIconSize(sz);
	toolButtonEdit->setIconSize(sz);
	toolButtonFirst->setIconSize(sz);
	toolButtonInsert->setIconSize(sz);
	toolButtonLast->setIconSize(sz);
	toolButtonRefresh->setIconSize(sz);
}


void TreeSqlWidget::setFilter(QString value){
	filter = value;
}

void TreeSqlWidget::setInsertValue(QStringList Name){
	insrec = Name;
}

void TreeSqlWidget::setRootValue(QStringList Name){
	root = Name;
}

void TreeSqlWidget::insertNode(QTreeWidgetItem *item, QStringList ins ){

	if (item==NULL){
		QMessageBox::critical(0,tr("Ошибка вставки"),tr("Не выбран раздел для вставки записи"));
		return;
	}

	// Вставляем ID
	QString str = "";
			//QString("%1=' ', ").arg(id);
	QString getid = QString("");
	// Вставляем Данные
	for (int i=0;i<head.count();i++){
		str = str+QString("'%1', ").arg(ins.at(i));
		getid = getid+QString("%1='%2' and ").arg(head.at(i)).arg(ins.at(i));
	}

	// Вставляем данные по умолчанию
	for (int i=0;i<insrec.count();i=i+2){
		str = str+QString("'%1', ").arg(insrec.at(i+1));
		getid = getid+QString("%1='%2' and ").arg(insrec.at(i)).arg(insrec.at(i+1));
	}
	// Вставляем родителя
	if (item->text(item->columnCount()-1)!=QString("NULL")){
		str=str+QString("'%1'").arg(item->text(item->columnCount()-1));
		getid=getid+QString("%1='%2'").arg(par).arg(item->text(item->columnCount()-1));
	}else{
		str=str+QString("%1").arg("NULL");
		getid=getid+QString("%1 IS NULL").arg(par);
	}


	QSqlQuery query;

	query.exec(QString("SELECT * FROM %1 WHERE %2").arg(tableName).arg(getid));
	if (!query.next()){

		if (query.exec(QString("INSERT INTO %1 (name, ppo_id, parent) VALUES (%2)").arg(tableName).arg(str))){
			query.exec(QString("SELECT * FROM %1 WHERE %2").arg(tableName).arg(getid));

			int fieldID = query.record().indexOf(id);
			while (query.next()){
				int i=0;
				QTreeWidgetItem *newitem = new QTreeWidgetItem(item);
				foreach(QString str,head){
					QString name = query.value(query.record().indexOf(head.at(i))).toString();
					newitem->setText(i, name);
					i++;
				}
				QString ID = query.value(fieldID).toString();
				newitem->setText(i, ID);
			}
		}else{
			QMessageBox::critical(0,tr("Ошибка вставки"),tr("Невозможно вставить запись\n")+query.lastError().text());

		}
	}else{
		QMessageBox::critical(0,tr("Ошибка вставки"),tr("Такая запись уже существует"));
		return;
	}

	// Перечитываем ветку
	/*
	for (int i=item->childCount()-1;i>=0;i--)
		item->removeChild(item->child(i));
	addNode(item,item->text(item->columnCount()-1));*/
}

void TreeSqlWidget::editNode(QTreeWidgetItem *item, QStringList ins ){

	if (item->text(item->columnCount()-1)==QString("NULL")){
		return;
	}

	QString str = QString("");
	// Вставляем Данные
	for (int i=0;i<head.count();i++){
		str = str+QString("%1='%2'").arg(head.at(i)).arg(ins.at(i));
		if (i!=head.count()-1)
			str = str+QString(", ");

	}

	QSqlQuery query;

	if (query.exec(QString("UPDATE %1 SET %2 WHERE %3='%4' ").arg(tableName).arg(str).arg(id).arg(item->text(item->columnCount()-1)))){
		int i=0;
		foreach(QString str,ins){
			item->setText(i, str);
			i++;
		}
	}else{
		QMessageBox::critical(0,tr("Ошибка изменения"),tr("Невозможно изменить запись"));
	}
	return;
}

void TreeSqlWidget::deleteNode(QTreeWidgetItem *item){

	if (item==NULL){
		QMessageBox::critical(0,tr("Ошибка удаления"),tr("Не выбран раздел для удаления"));
		return;
	}

	if (item->text(item->columnCount()-1)==QString("NULL")){
		return;
	}


	QSqlQuery query;
	if (query.exec(QString("DELETE FROM %1 WHERE %2='%3'").arg(tableName).arg(id).arg(item->text(item->columnCount()-1))))
		delete item;
	else
		QMessageBox::critical(0,tr("Ошибка удаления"),tr("Невозможно удалить запись"));
}
