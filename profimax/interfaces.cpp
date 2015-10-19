#include "interfaces.h"

QWidget* SpravInterface::getSubWindowAll(QString sprav)
{
	return new QWidget();
}

QWidget* SpravInterface::getSubWindow(QString sprav)
{
	QWidget* widget = NULL;
	if (sprav.indexOf(ModulName) == 0) {
		widget = getSubWindowAll(sprav.mid(ModulName.count()).split("-").at(0));
		if (widget != NULL)
			widget->setObjectName(sprav.split("-").at(0));
	}
	return widget;

}

QWidget* SpravInterface::getSubWindow(QTreeWidgetItem* item)
{

	QWidget* widget = getSubWindowAll(nameWidget.value(item).split("-").at(0));
	if (widget != NULL)
		widget->setObjectName(getWidgetName(item).split("-").at(0));
	return widget;
}

QWidget* SpravInterface::callReportPlugin(QString sprav)
{
	//QMessageBox::critical(0,"","Function plugin does not exist");
	return new QWidget();
}

void SpravInterface::setMainWindow(QMainWindow* m)
{
	mainWindow = m;
}

/*void SpravInterface::setListSprav(ListSpravInterface lspr){
  listSprav = lspr;
}*/

void SpravInterface::initPlug()
{

}

void SpravInterface::setTabWidget(QTabWidget* tab)
{
	tabWidgetMDI = tab;
}

void SpravInterface::addTabWidget(QWidget* widget, QString text)
{

	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (text == (tabWidgetMDI->tabText(i))) {
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	int index = tabWidgetMDI->addTab(widget, text);
	tabWidgetMDI->setCurrentIndex(index);
}

QList<QTreeWidgetItem*> SpravInterface::getSpravItems()
{
	QList<QTreeWidgetItem*> items;
	return items;
}

void SpravInterface::setWidgetName(QTreeWidgetItem* item, QString wname)
{
	nameWidget.insert(item, wname);
}

QString SpravInterface::getWidgetName(QTreeWidgetItem* item)
{
	if (ModulName + nameWidget.value(item) == NULL)
		return "";
	else
		return ModulName + nameWidget.value(item);
}

QTreeWidgetItem* SpravInterface::getItem(QString sprav)
{
	return nameWidget.key(sprav.mid(ModulName.count()));
}

bool SpravInterface::dragTreeWidget(QDropEvent* event, QTreeWidget* tree)
{
	return false;
}

void SpravInterface::callTabAction(QWidget* widget, QTreeWidgetItem* item)
{
	return;
}

void SpravInterface::contextMenu(QContextMenuEvent* event)
{
	return;
}

QList<QTreeWidgetItem*> SpravInterface::getAdminItems()
{
	QList<QTreeWidgetItem*> items;
	return items;
}
