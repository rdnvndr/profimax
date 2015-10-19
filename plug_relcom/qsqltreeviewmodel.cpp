#include "qsqltreeviewmodel.h"
#include <QtGui>

#define START_FIELD_SETDATA          2
#define NO_USED_SYSTEM_FIELD       2
//
QSqlTreeViewModel::QSqlTreeViewModel(QString tableName, QString primaryFieldName, QString parentFieldName, int id, QObject *parent):QStandardItemModel()
{
	QSqlTreeViewModel::primaryFieldName.clear();
	QSqlTreeViewModel::primaryFieldName.append(primaryFieldName);

	QSqlTreeViewModel::parentFieldName.clear();
	QSqlTreeViewModel::parentFieldName.append(parentFieldName);

	QSqlTreeViewModel::tableName.clear();
	QSqlTreeViewModel::tableName.append(tableName);

	QModelIndex item=index(0,0);
	//
	QSqlQuery query;
	query.prepare( QString("SELECT %1 FROM %2 WHERE %3 = %4")
				   .arg(QSqlTreeViewModel::primaryFieldName)
				   .arg(QSqlTreeViewModel::tableName)
				   .arg(QSqlTreeViewModel::parentFieldName)
				   .arg(id) );
	query.exec();

	QSqlTreeViewModel::primaryFieldNo = query.record().indexOf(QSqlTreeViewModel::primaryFieldName);

	if (query.next())
		QSqlTreeViewModel::init(query.value(QSqlTreeViewModel::primaryFieldNo).toInt(), item);
	//
}
//
QSqlTreeViewModel::~QSqlTreeViewModel()
{}
//
void QSqlTreeViewModel::init(int id, QModelIndex parent)
{
	QSqlQuery query(QString("SELECT * FROM %1 WHERE %2 = %3")
					.arg(QSqlTreeViewModel::tableName)
					.arg(QSqlTreeViewModel::primaryFieldName)
					.arg(id));
	query.exec();

	int fieldNo = query.record().indexOf(QSqlTreeViewModel::primaryFieldName);
	int countField = query.record().count();

	QSqlTreeViewModel::insertColumns(0, (countField-NO_USED_SYSTEM_FIELD), parent);

	QModelIndex item;
	if (query.next())
	{

		QSqlTreeViewModel::insertRow(QSqlTreeViewModel::rowCount(parent), parent);

		int fieldNoSetData = START_FIELD_SETDATA;
		while (fieldNoSetData <= countField)
		{
			item=index(rowCount(parent)-1, fieldNoSetData, parent);
			setData(item, query.value(fieldNoSetData-1));
			fieldNoSetData += 1;
		}
		QSqlQuery query;
		query.prepare( QString("SELECT %1 FROM %2 WHERE %3 = %4")
					   .arg(QSqlTreeViewModel::primaryFieldName)
					   .arg(QSqlTreeViewModel::tableName)
					   .arg(QSqlTreeViewModel::parentFieldName)
					   .arg(id) );
		query.exec();
		if (query.next())
		{
			QSqlTreeViewModel::insertColumns(0, (countField-NO_USED_SYSTEM_FIELD), item);
			do
			{
				init(query.value(QSqlTreeViewModel::primaryFieldNo).toInt(), item);
			}
			while (query.next());
		}
	}
}
//
