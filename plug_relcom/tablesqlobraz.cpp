#include "tablesqlobraz.h"

TableSqlObraz::TableSqlObraz(){
		setEditStrategy(QSqlTableModel::OnFieldChange);
		setTable(tr("prof_edu"));

		select();

		QStringList headName;
		headName << "name";
		setSortColumn(headName);

		setHeader("name", tr("Образование"));
		show();
}
