/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGSWITCHPPO_H
#define	DIALOGSWITCHPPO_H
#include "ui_dialogswitchppo.h"
#include <QDataWidgetMapper>
#include <QtSql>


class DialogSwitchPPO: public QDialog, public Ui::DialogSwitchPPO{
		Q_OBJECT


	public:
		QString indexPPO;
		QSqlTableModel* model;
		QSqlTableModel* relModel;
		QDataWidgetMapper* mapper;
		DialogSwitchPPO(QWidget* pwgt = 0);
		void setPPO(QString ppo);

};

#endif
