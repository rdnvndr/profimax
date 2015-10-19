/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGEDITOTDEL_H
#define	DIALOGEDITOTDEL_H
#include "ui_dialogeditotdel.h"
#include <QtGui>


class DialogEditOtdel: public QDialog, public Ui::DialogEditOtdel{
		Q_OBJECT

	public:
		DialogEditOtdel(QWidget* pwgt = 0);

};

#endif
