#ifndef QSQLTREEVIEWMODEL_H
#define QSQLTREEVIEWMODEL_H
//

#include <QtSql>
#include <QtGui>

//
class QSqlTreeViewModel : public QStandardItemModel
{
Q_OBJECT

public:
	QSqlTreeViewModel( QString tableName, QString primaryFieldName, QString parentFieldName, int id = 0, QObject *parent = 0 );
	virtual ~QSqlTreeViewModel();

	void init( int id, QModelIndex parent = QModelIndex() );

private:
	QString  primaryFieldName;
	QString  parentFieldName;
	QString  tableName;
	int primaryFieldNo;
};
#endif
