#ifndef PROFIMAXRELATIONALMODEL_H
#define PROFIMAXRELATIONALMODEL_H

#include <QSqlRelationalTableModel>

class ProfimaxRelationalModel : public QSqlRelationalTableModel
{
public:
    ProfimaxRelationalModel();
	bool setData(const QModelIndex &index, const QVariant &value, int role);
protected:
     QString selectStatement() const;
};

#endif // ProfimaxRelationalModel_H
