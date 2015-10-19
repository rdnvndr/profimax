/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "dialogppo.h"
#include <QtGui>
#include <QtSql>

DialogPPO::DialogPPO(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);

	QObject::connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(onButtonOK()));
	QObject::connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(onButtonCancel()));

}

void DialogPPO::onButtonOK() {
	model->submit();
	this->close();
}

void DialogPPO::onButtonCancel() {

	model->revert();
	this->close();
}
void DialogPPO::setModel(QSqlTableModel* modd) {

	model = modd;
	mapper = new QDataWidgetMapper(this);
	mapper->setModel(model);
	mapper->addMapping(lineEditName, model->fieldIndex("name"));
	mapper->addMapping(lineEditCode, model->fieldIndex("code"));
	mapper->addMapping(lineEditPost, model->fieldIndex("post"));
	mapper->addMapping(lineEditAdress, model->fieldIndex("adress"));
	mapper->addMapping(checkBoxUFace, model->fieldIndex("urface"));
	mapper->addMapping(lineEditRuk, model->fieldIndex("ruk"));
	mapper->addMapping(lineEditTelRuk, model->fieldIndex("ruktel"));
	mapper->addMapping(lineEditPred, model->fieldIndex("pred"));
	mapper->addMapping(lineEditTelPred, model->fieldIndex("predtel"));
	mapper->addMapping(lineEditZam, model->fieldIndex("zam"));
	mapper->addMapping(lineEditTelZam, model->fieldIndex("zamtel"));
	mapper->addMapping(lineEditChPred, model->fieldIndex("chpred"));
	mapper->addMapping(lineEditChProf, model->fieldIndex("chprof"));
	mapper->addMapping(lineEditBUp, model->fieldIndex("bup"));
	mapper->addMapping(lineEditBDown, model->fieldIndex("bdown"));
	mapper->addMapping(lineEditSubject, model->fieldIndex("subject"));
	mapper->addMapping(lineEditOtrasl, model->fieldIndex("otrasl"));
	mapper->addMapping(lineEditProf, model->fieldIndex("profname"));
	mapper->addMapping(lineEditUrAdress, model->fieldIndex("ur_adress"));
	mapper->addMapping(lineEditNameBank, model->fieldIndex("name_bank"));
	mapper->addMapping(lineEditAdressBank, model->fieldIndex("adress_bank"));
	mapper->addMapping(spinBoxINN, model->fieldIndex("inn"));
	mapper->addMapping(lineEditRasSchet, model->fieldIndex("rasch_schet"));
	mapper->addMapping(lineEditKorrSchet, model->fieldIndex("korr_schet"));
	mapper->addMapping(lineEditBIK, model->fieldIndex("bik"));
}
