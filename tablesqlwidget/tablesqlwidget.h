/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef TABLESQLWIDGET_H
#define	TABLESQLWIDGET_H
#include "ui_tablesqlwidget.h"
#include <QtGui>
#include <QtSql>
#include "profimaxrelationalmodel.h"

class TableSqlWidget: public QWidget, public Ui::FormSql{
		Q_OBJECT

	public:
                ProfimaxRelationalModel *fmodel;
                ProfimaxRelationalModel *model;
		QSqlDatabase ddb;
		QSqlQuery* query;
		QStandardItemModel* amodel;
		QStringList fieldNameBool;
		QStringList insrec;
		QString filter;

		TableSqlWidget(QWidget* pwgt = 0);
		void show();
		void select();
		void setTable(const QString & tableName);
		void setHeader(const QString & col, const QString & colname);
		void removeColumn(const QString & col);
		void setSortColumn(QStringList stl);
		void setFieldNameBool(QStringList Name);
		void setButtoIconSize(int size);
		void setMargin(int size);
		void setInsertValue(QStringList Name);
		void setFilter(QString value);
		void setEditStrategy(QSqlTableModel::EditStrategy strategy );
		QVariant getCurrentValue(QString s);
		void setCurrentValue(QString s,QVariant v);
		void setRelation(QString mycol,QString table,QString linkcol,QString showcol);

		void disconnectButtonInsert();
		void disconnectButtonDelete();
		void disconnectButtonRefresh();
	private:
		QStringList columnshow;

	public slots:
		virtual void onClickToolButtonRefresh();
		virtual int  onClickToolButtonInsert();
		virtual void onClickToolButtonDelete();

		virtual void onClickToolButtonFilter();
		virtual void onClickToolButtonFilterAdd();
		virtual void onClickToolButtonFilterRemove();
		virtual void onClickToolButtonFilterApply();
		virtual void onClickToolButtonFilterCancel();

};

#endif
