#include "tablesqldolgn.h"

TableSqlDolgn::TableSqlDolgn(){
		setEditStrategy(QSqlTableModel::OnFieldChange);
		setTable(tr("prof_dolgn"));

		select();

		QStringList headName;
		headName << "name";
		setSortColumn(headName);

		setHeader("name", tr("Должность"));
		show();
}
