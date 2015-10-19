/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGMSG_H
#define	DIALOGMSG_H
#include "ui_dialogmsg.h"
#include <QtGui>
#include <QtSql>
#include "comboboxdelegmail.h"


class DialogMsg: public QMainWindow, public Ui::DialogMsg{
		Q_OBJECT

	public:
                QStandardItemModel* amodel;
                ProfimaxRelationalModel *model;
		QString removemsgid;

                DialogMsg(QWidget* pwgt = 0);
                void setModel(ProfimaxRelationalModel *model);
		void insertMessage(QStringList field, QStringList val);
		void loadMessage(QString msgid);
		void addRowAttr(QString name, QString value,int row);



        private slots:

		void closeEvent(QCloseEvent *event);
                void onActionSend();
                void onActionSave();
		void onActionPrint();
		void onActionPrintPreview();
		void onActionExportPDF();

		void onActionCut();
		void onActionCopy();
		void onActionPaste();

		void onActionUndo();
		void onActionRedo();
		void onActionSelectAll();
		void onFocusChanged(QWidget* old,QWidget* now);

};

#endif
