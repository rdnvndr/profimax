/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef TOOLBOXPAGEBUTTON_H
#define	TOOLBOXPAGEBUTTON_H
#include "ui_toolboxpagebutton.h"
#include <QtGui>


class QToolBoxPageButton: public QWidget, public Ui::QToolBoxPageButton{
		Q_OBJECT

	public:
		QToolBoxPageButton(QWidget* pwgt = 0);

};

#endif
