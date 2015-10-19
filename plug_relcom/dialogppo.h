/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGPPO_H
#define	DIALOGPPO_H
#include "ui_dialogppo.h"
#include <QDataWidgetMapper>
#include <QtSql>

class DialogPPO: public QDialog, public Ui::DialogPPO{
		Q_OBJECT


	public:
		DialogPPO(QWidget* pwgt = 0);

		QSqlTableModel *model;
		QItemSelectionModel *selectionModel;
		QDataWidgetMapper *mapper;
		int typeIndex;

		void setModel(QSqlTableModel* modd);

	private slots:
		void onButtonOK();
		void onButtonCancel();

};

#endif
