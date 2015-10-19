#include "profimaxrelationalmodel.h"
#include <QtGui>
#include <QtSql>


ProfimaxRelationalModel::ProfimaxRelationalModel()
{

}

QString ProfimaxRelationalModel::selectStatement() const
{
    QString query = QSqlRelationalTableModel::selectStatement();



    //QMessageBox::critical(0,"",r->field(1));

    //query.replace("city_name_2","city_name");
    int i =query.indexOf(" ");
    int from = query.indexOf(" FROM ");
    int j;
    int index=0;
    QString fields = QString("");

    while (i<from){
	j=i+1;
	i=query.indexOf(",",i+1);

	if (i>from or i==-1)
	    i=from;

	QString f = query.mid(j,i-j);

	if (f.indexOf("relTblAl")!=-1){
	    if (relation(index).isValid()){
		f=f.left(f.indexOf(" AS "));
		f=f+QString(" AS ")+QString("\"%1\"").arg(this->record().fieldName(index));
	    }
	}

	fields += QString("%1,").arg(f);
	//QMessageBox::critical(0,"",f);
	index++;

    }
    fields.chop(1);
    query = QString("SELECT ")+fields+query.mid(from);

    //QMessageBox::critical(0,"",query);

    return query;
 }

bool ProfimaxRelationalModel::setData(const QModelIndex &index, const QVariant &value,
									   int role)
{

	return QSqlTableModel::setData(index, value, role);
}


