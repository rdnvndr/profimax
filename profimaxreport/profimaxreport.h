/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef PROFIMAXREPORT_H
#define	PROFIMAXREPORT_H
#include "ui_profimaxreport.h"
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtGui>
#include <QtSql>
#include <QApplication>

class ProfimaxReport: public QWidget, public Ui::ProfimaxReport{
		Q_OBJECT

	public:
		QStringList insparam;
		QString fileform;
		QString reportname;

		ProfimaxReport(QWidget* pwgt = 0);

		void setInsertParam(QStringList Name);
		QStringList getInsertParam();
		QString getReportName();
		void setReportName(QString reportname);
		QString Replace(QString str);


	public slots:
		bool load(const QString &f);
		void filePrint();
		void filePrintPreview();
		void filePrintPdf();
		void printPreview(QPrinter *printer);
};

#endif
