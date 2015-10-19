/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "dialogswitchppo.h"

DialogSwitchPPO::DialogSwitchPPO(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);

	model = new QSqlTableModel();
	model->setTable("prof_ppo");
	model->select();
	comboBoxPPO->setModel(model);
	comboBoxPPO->setModelColumn(model->fieldIndex("name"));
}

void DialogSwitchPPO::setPPO(QString ppo){
	indexPPO = ppo;
	QSqlQuery query;
	query.exec(QString("SELECT * FROM prof_ppo WHERE ppo_id='%1'").arg(indexPPO));
	int cur=-1;
	while(query.next())
		cur=comboBoxPPO->findText(query.record().value(query.record().indexOf("name")).toString());
	if (cur>=0)
		comboBoxPPO->setCurrentIndex(cur);

}

