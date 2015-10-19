#include "mailsqlrelationaldelegate.h"



void MailSqlRelationalDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem myoption = option;

	bool val = index.model()->data(index.sibling(index.row(),3)).toBool();
	if (val!=true)
	    myoption.font.setBold(true);

	/*
	if (index.model()->data(index, Qt::EditRole).type() == QVariant::Bool){
	    myoption.font.setBold(true);
	    QMessageBox::critical(0,"",QString("%1").arg(index.column()));
	}*/

	ProfimaxDelegate::paint(painter, myoption, index);
}
