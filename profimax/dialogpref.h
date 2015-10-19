/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGPREF_H
#define	DIALOGPREF_H
#include "ui_dialogpref.h"
#include <QtGui>


class DialogPref: public QDialog, public Ui::DialogPref{
		Q_OBJECT

	public:
		DialogPref(QWidget* pwgt = 0);

};

#endif
