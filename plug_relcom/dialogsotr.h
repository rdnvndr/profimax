/*
 *      mainwindow.h
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */

#ifndef DIALOGSOTR_H
#define	DIALOGSOTR_H
#include "ui_dialogsotr.h"
#include <QDataWidgetMapper>
#include <QToolTip>
#include "tablesqlwidget.h"
#include "qcboxbutton.h"

class DialogSotr: public QDialog, public Ui::DialogSotr{
		Q_OBJECT

	public:
		DialogSotr(QWidget* pwgt = 0);

		ProfimaxRelationalModel *model;
		QItemSelectionModel *selectionModel;
		QDataWidgetMapper *mapper;
		QString ppo;


		//ProfimaxRelationalModell *modelTel;
		//QDataWidgetMapper *mapperTel;

		int typeIndex;
		int mapidx;

                void setModel(ProfimaxRelationalModel* modd);
		void setCurrentIndex(int index);
		void submitAll();
		void revertAll();
		void setEditable(bool fl);
		bool checkInput(QWidget* widget,QString mes,int tab);
	public slots:
		void apply();
		void changeMember(int ci);
		void changeProfActiv(int ci);
		void changeSoc(int ci);
		void onChooseOtdel();
		void onChooseDolgn();
		void onChooseEdu();
		void onChooseFoto();

};

#endif
