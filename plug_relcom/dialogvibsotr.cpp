/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#include <QMessageBox>
#include "dialogvibsotr.h"

DialogVibSotr::DialogVibSotr(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);
	mapper = new QDataWidgetMapper(this);
}

void DialogVibSotr::setModel(ProfimaxRelationalModel* modd) {

	model = modd;

	// Page Профсоюз
	QSqlTableModel* relModel = model->relationModel(model->fieldIndex("member_id"));
	relModel->setFilter("not (member_id=1 or member_id=5)");
	comboBoxMember->setModel(relModel);
	comboBoxMember->setModelColumn(relModel->fieldIndex("name"));

	mapper->setModel(model);
	mapper->setItemDelegate(new QSqlRelationalDelegate(this));

	mapper->addMapping(lineEditLastName, model->fieldIndex("lastname"));
	mapper->addMapping(lineEditFirstName, model->fieldIndex("firstname"));
	mapper->addMapping(lineEditMidName, model->fieldIndex("midname"));
	mapper->addMapping(lineEditTabNum, model->fieldIndex("tabnum"));
	mapper->addMapping(dateEditDate, model->fieldIndex("date_in"));
	mapper->addMapping(comboBoxMember, model->fieldIndex("member_id"));

}

void DialogVibSotr::setCurrentIndex(int index){
	mapper->setCurrentIndex(index);
}

void DialogVibSotr::submitAll(){
	if (!model->submitAll()){
		revertAll();
		QMessageBox::critical(0,"",tr("Ошибка ввода данных"));
	}
}

void DialogVibSotr::revertAll(){
	model->revertAll();
}
