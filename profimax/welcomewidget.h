/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef WELCOMEWIDGET_H
#define	WELCOMEWIDGET_H
#include "ui_welcomewidget.h"
#include "flowlayout.h"
#include <QtGui>
#include "mytreewidget.h"
#include "welcometoolbutton.h"
#include "mainwindow.h"


class WelcomeWidget: public QWidget, public Ui::WelcomeWidget{
		Q_OBJECT

	public:
		FlowLayout *flowLayout;

		WelcomeWidget(QWidget* pwgt = 0);


		void dropEvent(QDropEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		WelcomeToolButton* addButton(QString text,QIcon icon);
		void restoreButton();

	public slots:
		void removeButton();

};

#endif
