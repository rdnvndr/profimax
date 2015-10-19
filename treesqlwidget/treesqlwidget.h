/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef TREESQLWIDGET_H
#define	TREESQLWIDGET_H
#include "ui_treesqlwidget.h"
#include <QtGui>
#include <QtSql>

class TreeSqlWidget: public QWidget, public Ui::TreeSqlWidget{
		Q_OBJECT

	public:
		QString tableName;
		QString filter;
		QStringList head;
		QString id;
		QString par;
		QStringList insrec;
		QStringList root;


		TreeSqlWidget(QWidget* pwgt = 0);
		void select();
		void setTable(const QString & tableName);
		void setHeader(QStringList stl);
		void setHeaderLabels(QStringList headers);
		void hideColumn(int col);

		void setButtonIconSize(int size);
		void setFilter(QString value);
		QString lineFields();
		void setNodeField(QString id,QString par);
		void addNode(QTreeWidgetItem* widget,QString parent);
		void insertNode(QTreeWidgetItem *item, QStringList ins );
		void editNode(QTreeWidgetItem *item, QStringList ins );
		void deleteNode(QTreeWidgetItem *item);
		void setInsertValue(QStringList Name);
		void setRootValue(QStringList Name);


	private:
		QStringList columnshow;

	public slots:
		virtual int onClickToolButtonInsert();
		virtual void onClickToolButtonDelete();
		virtual void onClickToolButtonEdit();
		virtual void onClickToolButtonRefresh();

		virtual void onClickToolButtonFilter();
		virtual void onClickToolButtonFilterAdd();
		virtual void onClickToolButtonFilterRemove();
		virtual void onClickToolButtonFilterApply();
		virtual void onClickToolButtonFilterCancel();

};

#endif
