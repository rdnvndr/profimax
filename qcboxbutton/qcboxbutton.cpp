/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "qcboxbutton.h"

QCBoxButton::QCBoxButton(QWidget* pwgt) : QWidget(pwgt) {
	setupUi(this);
	connect(toolButton,SIGNAL(clicked()),this,SIGNAL(clickChoose()));
}

void QCBoxButton::setModel( QAbstractItemModel * model ){
	comboBox->setModel(model);
}

void QCBoxButton::setModelColumn( int visibleColumn ){
	comboBox->setModelColumn(visibleColumn);
}

void QCBoxButton::setEditable( bool editable ){
	comboBox->setEditable(editable);
}

void QCBoxButton::setCurrentIndex ( int index ){
	comboBox->setCurrentIndex(index);
}

int QCBoxButton::currentIndex() const{
	return comboBox->currentIndex();
}

QAbstractItemModel * QCBoxButton::model () const{
	return comboBox->model();
}

QString QCBoxButton::currentText () const{
	return comboBox->currentText();
}
