/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "dialogabout.h"
#include "version.h"

DialogAbout::DialogAbout(QWidget* pwgt) : QDialog(pwgt) {
	setupUi(this);
        labelVer->setText(QString(tr("Версия %1")).arg(PROFIMAX_REV));

}

