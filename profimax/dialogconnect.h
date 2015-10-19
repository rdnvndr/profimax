/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGCONNECT_H
#define	DIALOGCONNECT_H
#include "ui_dialogconnect.h"
#include <QtGui>
#include <QtSql>

class DialogConnect: public QDialog, public Ui::DialogConnect{

		Q_OBJECT
	public:
		DialogConnect(QWidget* pwgt = 0);
	public slots:
		void onClickButtonProp();
};

#endif
