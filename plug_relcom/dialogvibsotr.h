/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGVIBSOTR_H
#define	DIALOGVIBSOTR_H
#include "ui_dialogvibsotr.h"
#include <QtGui>
#include <QtSql>
#include <QDataWidgetMapper>
#include "profimaxrelationalmodel.h"


class DialogVibSotr: public QDialog, public Ui::DialogVibSotr{
		Q_OBJECT

	public:
                ProfimaxRelationalModel *model;
		QDataWidgetMapper *mapper;

		DialogVibSotr(QWidget* pwgt = 0);
                void setModel(ProfimaxRelationalModel* modd);
		void setCurrentIndex(int index);
		void submitAll();
		void revertAll();
};

#endif
