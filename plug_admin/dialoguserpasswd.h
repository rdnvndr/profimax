/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGUSERPASSWD_H
#define	DIALOGUSERPASSWD_H
#include "ui_dialoguserpassword.h"
#include <QtGui>


class DialogUserPasswd: public QDialog, public Ui::DialogUserPasswd{
		Q_OBJECT

	public:
		DialogUserPasswd(QWidget* pwgt = 0);

};

#endif
