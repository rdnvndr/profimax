#ifndef PLUG_WORKFLOW_H
#define PLUG_WORKFLOW_H

#include "../profimax/interfaces.h"


class WorkflowPlugin :  public SpravInterface
{
	Q_OBJECT
	Q_INTERFACES(SpravInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.Roand.Profimax.workflow" FILE "workflow.json")
#endif


public:
	// SpravInterface
	QHash <QTreeWidgetItem*,QString> filter;

	WorkflowPlugin();

	QWidget* getSubWindowAll(QString sprav);
	QList<QTreeWidgetItem *> getSpravItems();
	bool dragTreeWidget(QDropEvent *event,QTreeWidget* tree);
	void callTabAction(QWidget* widget,QTreeWidgetItem* item);

public slots:
	virtual void contextMenu(QContextMenuEvent *event);
	virtual void renameFilter();
	virtual void removeFilter();


};

#endif
