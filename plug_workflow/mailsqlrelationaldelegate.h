#ifndef MAILSQLRELATIONALDELEGATE_H
#define MAILSQLRELATIONALDELEGATE_H
#include "profimaxdelegate.h"

class MailSqlRelationalDelegate : public ProfimaxDelegate
{
	//Q_OBJECT
    public:
	//virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // MAILSQLRELATIONALDELEGATE_H
