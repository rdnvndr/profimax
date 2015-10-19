#ifndef TREESQLOTDEL_H
#define TREESQLOTDEL_H

#include "treesqlwidget.h"

#include "qsqltreeviewmodel.h"
#include "dialogvibsotr.h"
#include "dialogeditotdel.h"

class TreeSqlOtdel : public TreeSqlWidget{

	public:
		QString ppo;
		TreeSqlOtdel(QString p);
	public slots:
		virtual int onClickToolButtonInsert();
		virtual void onClickToolButtonDelete();
		virtual void onClickToolButtonEdit();
};

#endif // TREESQLOTDEL_H
