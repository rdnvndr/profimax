/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "dialogconnect.h"
#include <QtGui>
#include <QtSql>

DialogConnect::DialogConnect(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);

	textLabelHostname->hide();
	editHostname->hide();
	textLabelBD->hide();
	editDatabase->hide();
	textLabelPort->hide();
	portSpinBox->hide();
	textLabelDrv->hide();
	comboDriver->hide();
	line->hide();

	QStringList drivers = QSqlDatabase::drivers();

	// remove compat names
	drivers.removeAll("QMYSQL3");
	drivers.removeAll("QOCI8");
	drivers.removeAll("QODBC3");
	drivers.removeAll("QPSQL7");
	drivers.removeAll("QTDS7");

	drivers.removeAll("QSQLITE");
	drivers.removeAll("QOCI");
	drivers.removeAll("QODBC");
	//drivers.removeAll("QPSQL");
	drivers.removeAll("QTDS");

	comboDriver->addItems(drivers);

	//connect(okButton, SIGNAL(clicked()), this, SLOT(createConnect()));
	//connect(cancelButton, SIGNAL(clicked()), this, SLOT(QApplication::quit()));
	this->adjustSize();
	connect(pushButtonProp, SIGNAL(clicked()), this, SLOT(onClickButtonProp()));
}

void DialogConnect::onClickButtonProp(){
    if (textLabelHostname->isHidden()){
	pushButtonProp->setText(tr("Кратко"));
	textLabelHostname->show();
	editHostname->show();
	textLabelBD->show();
	editDatabase->show();
	textLabelPort->show();
	portSpinBox->show();
	textLabelDrv->show();
	comboDriver->show();
	line->show();
    } else {
	pushButtonProp->setText(tr("Подробно"));
	textLabelHostname->hide();
	editHostname->hide();
	textLabelBD->hide();
	editDatabase->hide();
	textLabelPort->hide();
	portSpinBox->hide();
	textLabelDrv->hide();
	comboDriver->hide();
	line->hide();

    }
    this->adjustSize();
}
