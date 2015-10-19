#include "treesqlotdel.h"
#include <QMessageBox>

TreeSqlOtdel::TreeSqlOtdel(QString p){

		ppo=p;

		setTable("prof_otdel");
		setNodeField("otdel_id","parent");

		QStringList fields;
		fields << tr("name");
		setHeader(fields);

		QStringList headers;
		headers << tr("Наименования")<< tr("Наименования");
		setHeaderLabels(headers);
		hideColumn(1);

		if (ppo!=tr(""))
			setFilter(QString("ppo_id='%1'").arg(ppo));

	   QStringList rootval;
	   rootval << tr("Подразделения")<< tr("Подразделения");
	   setRootValue(rootval);

	   QStringList insval;
	   insval << "ppo_id"<<QString("%1").arg(p);
	   setInsertValue(insval);

	   select();
}

void TreeSqlOtdel::onClickToolButtonDelete(){

	TreeSqlWidget *tree = qobject_cast<TreeSqlWidget *>(sender()->parent());
	tree->deleteNode(tree->treeWidget->currentItem());
}

void TreeSqlOtdel::onClickToolButtonEdit(){


	TreeSqlWidget *tree = qobject_cast<TreeSqlWidget *>(sender()->parent());
	QTreeWidgetItem* item =  tree->treeWidget->currentItem();
	if (item->text(item->columnCount()-1)==QString("NULL")){

		return;
	}

	DialogEditOtdel* windowOtdel = new DialogEditOtdel(this->parentWidget());
	windowOtdel->lineEditNameOtdel->setText(item->text(0));

	if (windowOtdel->exec()==QDialog::Accepted) {

		QStringList headers;
		headers << windowOtdel->lineEditNameOtdel->text();
		tree->editNode(item,headers);
	}

	delete windowOtdel;


}

int TreeSqlOtdel::onClickToolButtonInsert(){

	if (ppo==QString("00000000-0000-0000-0000-000000000000")){
		QMessageBox::critical(0,tr("Ошибка вставки записи"),tr("Не выбран ППО"));
		return 0;
	}

	DialogEditOtdel* windowOtdel = new DialogEditOtdel(this->parentWidget());

	if (windowOtdel->exec()==QDialog::Accepted) {
		TreeSqlWidget *tree = qobject_cast<TreeSqlWidget *>(sender()->parent());
		QStringList headers;
		headers << windowOtdel->lineEditNameOtdel->text();
		tree->insertNode(tree->treeWidget->currentItem(),headers);
	}

	delete windowOtdel;
	return 0;


}
