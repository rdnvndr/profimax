/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "dialogforward.h"

DialogForward::DialogForward(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);

	model = new QSqlTableModel();
	model->setTable("prof_user");
	model->setFilter("not user_id='00000000-0000-0000-0000-000000000000'");
	model->select();
	comboBoxUser->setModel(model);
	comboBoxUser->setModelColumn(model->fieldIndex("lastname"));
}


