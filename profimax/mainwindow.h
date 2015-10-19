/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include  "ui_mainwindow.h"
#include  "dialogabout.h"
#include  "dialogconnect.h"

#include  "dialogpref.h"
#include  "toolboxpagebutton.h"
#include  "tablesqlwidget.h"
#include  "profimaxreport.h"
#include  "dialogswitchppo.h"
#include  "dialoguserpasswd.h"
#include  "welcomewidget.h"
#include  "interfaces.h"
#include  "updateftp.h"
#include  "spravtoolbutton.h"
#include  "mytreewidget.h"

#include <QtGui>
#include <QDir>
#include <QMainWindow>
#include <QStringList>
#include <QtSql>
#include <QtNetwork>


class MainWindow: public QMainWindow, public Ui::MainWindow{
		Q_OBJECT


	private:

		// Для подключения модулей
		QSettings*	settings;

		//QDir		pluginsDir;
		//QStringList 	pluginFileNames;

		QSqlDatabase 	db;

		void createPlugObject(QObject *plugin);

		void addToToolBox(QObject *plugin, QVBoxLayout *box);

	public slots:
		bool onActionConnect();

	private slots:
		void callFuncTreeItem(QTreeWidgetItem* item, int column);
		void callFuncAction();
		void callFuncTreeMain(QTreeWidgetItem* item, int column);

		void closeTab(int index);
		void showTabWelcome();

		void onActionAbout();
		void onActionHelp();
		void onActionPref();

		void onCut();
		void onCopy();
		void onPaste();

		void onUndo();
		void onRedo();
		void onSelectAll();
		void onFocusChanged(QWidget* old,QWidget* now);
		void currentChanged(int index);
		void ftpDownDir(UpdateFTP* updateFtp,QString direct,QString ftpDir);
		void cleanFileUpdate(QString direct);


		void onActionPrint();
		void onActionPrintPreview();
		void onActionExportPDF();

		void onActionUpdate();

		void onActionOpenReport();
		void onEditOpenReport();
		void onActionSaveReport();
		void closeEvent(QCloseEvent *event);
		void onActionPlug();

	public:
		ListSpravInterface listSprav;
		MainWindow(QMainWindow* pwgt = 0);

		void loadPlugins();
		void readSettings();
		void writeSettings();

		void closeTabAll();
};

#endif
