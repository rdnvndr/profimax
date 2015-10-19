/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef QCBOXBUTTONL_H
#define	QCBOXBUTTONL_H
#include "ui_qcboxbutton.h"
#include <QtGui>


class QCBoxButton: public QWidget, public Ui::QCBoxButton{
		Q_OBJECT

	public:

		QCBoxButton(QWidget* pwgt = 0);

		int currentIndex () const;
		void setCurrentIndex (int index);
		void setModel ( QAbstractItemModel * model );
		QAbstractItemModel* model() const;
		void setModelColumn ( int visibleColumn );
		void setEditable ( bool editable );
		QString currentText () const;

	 signals:
		void clickChoose();

};

#endif
