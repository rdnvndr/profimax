
 #ifndef COMBOBOXFILELDDELEGATE_H
 #define COMBOBOXFILELDDELEGATE_H

#include <QtGui>
#include <QtSql>
#include "profimaxrelationalmodel.h"

class ComboBoxFileldDelegate: public QItemDelegate{

	 Q_OBJECT

 public:

	QList<int> fieldName;
        ProfimaxRelationalModel *model;

	//ComboBoxDelegate(QWidget *widget = 0);

	virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
	virtual void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
	virtual  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
	void setFieldName(QList<int> Name);
        void setModel(ProfimaxRelationalModel *model);
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

 };

 #endif

