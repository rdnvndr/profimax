/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGABOUT_H
#define	DIALOGABOUT_H
#include "ui_dialogabout.h"
#include <QtGui>


class DialogAbout: public QDialog, public Ui::DialogAbout{
		Q_OBJECT

	public:
		DialogAbout(QWidget* pwgt = 0);

};

#endif
