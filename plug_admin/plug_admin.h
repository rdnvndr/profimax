#ifndef PLUG_ADMIN_H
#define PLUG_ADMIN_H

#include "../profimax/interfaces.h"

class AdminPlugin :  public SpravInterface
{
	Q_OBJECT
	Q_INTERFACES(SpravInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.Roand.Profimax.admin" FILE "admin.json")
#endif

public:
	// SpravInterface

	AdminPlugin();

	void setRItems(QTreeWidgetItem* item, SpravInterface * iSprav);
	QWidget* getSubWindowAll(QString sprav);
	QList<QTreeWidgetItem *> getSpravItems();
	QVariant GetQueryResult(const QString &query);

};

#endif
