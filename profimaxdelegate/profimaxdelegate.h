
 #ifndef PROFIMAXDELEGATE_H
 #define PROFIMAXDELEGATE_H

#include <QtGui>
#include <QtSql>
#include "profimaxrelationalmodel.h"

class ProfimaxDelegate: public QSqlRelationalDelegate{

	 Q_OBJECT

 public:
	QStringList fieldName;
	

	virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
	virtual void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
	virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setFieldName(QStringList Name);
 };

 #endif

