/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGUPDATE_H
#define	DIALOGUPDATE_H
#include "ui_dialogupdate.h"
#include <QtGui>


class DialogUpdate: public QDialog, public Ui::DialogUpdate{
		Q_OBJECT

	public:
		DialogUpdate(QWidget* pwgt = 0);

};

#endif
