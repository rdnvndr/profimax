#ifndef DIALOGKVP_H
#define DIALOGKVP_H
#include "ui_dialogkvp.h"
#include <QDataWidgetMapper>
#include "tablesqlwidget.h"
#include "qcboxbutton.h"


class DialogKvp : public QDialog, public Ui::DialogKvp
{
	Q_OBJECT
    public:
	ProfimaxRelationalModel *model;
	ProfimaxRelationalModel *modelins;

	QItemSelectionModel *selectionModel;
	QDataWidgetMapper *mapper;
	QDataWidgetMapper *mapperSotr;
	ProfimaxRelationalModel *modelSotr;
	QString *ppo;
	int typeIndex;
	int mapidx;

	DialogKvp(QWidget* pwgt = 0);

	void setModel(ProfimaxRelationalModel* modd);
	void setCurrentIndex(int index);
	void submitAll();
	void revertAll();
	bool checkInput(QWidget* widget,QString mes,int tab);

public slots:
	void apply();

};

#endif // DIALOGKVP_H
