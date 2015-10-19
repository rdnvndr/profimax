#ifndef PLUG_RELCOM_H
#define PLUG_RELCOM_H

#include <QObject>
#include "../profimax/interfaces.h"
#include <QtSql>
#include <QtGui>
#include "tablesqlwidget.h"
#include "dialogsotr.h"
#include "profimaxreport.h"

class RelcomPlugin : public SpravInterface
{
	Q_OBJECT
	Q_INTERFACES(SpravInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.Roand.Profimax.relcom" FILE "relcom.json")
#endif
public:
	// SpravInterface
	QString indexPPO;
	RelcomPlugin();

	QWidget* callReportPlugin(QString sprav);
	void callFuncPlugin(QString sprav);
	QWidget* getSubWindowAll(QString sprav);
	QList<QTreeWidgetItem *> getSpravItems();
	QList<QTreeWidgetItem *> getAdminItems();
	QVariant GetQueryResult(const QString &query);
	void initPlug();

private slots:
	void editDialogSotr();
	void vibDialogSotr();
	void editDialogKvp();
	void editDialogPPO();
};

#endif
