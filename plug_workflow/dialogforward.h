/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGFORWARD_H
#define	DIALOGFORWARD_H
#include "ui_dialogforward.h"
#include <QtGui>
#include <QtSql>
#include <QDataWidgetMapper>


class DialogForward: public QDialog, public Ui::DialogForward{
		Q_OBJECT


	public:
		QString indexPPO;
		QSqlTableModel* model;
		QSqlTableModel* relModel;
		QDataWidgetMapper* mapper;
		DialogForward(QWidget* pwgt = 0);
};

#endif
