#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
class QWidget;
class QString;
class QStringList;
class ProfimaxReport;
QT_END_NAMESPACE

class SpravInterface;
typedef QList <SpravInterface *> ListSpravInterface;

class SpravInterface: public QObject
{
		Q_OBJECT

	public:
		QHash <QTreeWidgetItem*, QString> nameWidget;
		QString ModulName;
		QIcon ModulIcon;
		QSqlDatabase* db;
		QTabWidget*  tabWidgetMDI;
		QMainWindow* mainWindow;
		ListSpravInterface* listSprav;

		//-- Пользовательские функции
		virtual			 ~SpravInterface() {}

		// Вызов окна справочника
		virtual QWidget* getSubWindowAll(QString sprav);

		// Вызов справочника отчетов
		virtual QWidget* callReportPlugin(QString sprav);

		// Обработка переноса объекта дерева
		virtual bool	 dragTreeWidget(QDropEvent* event, QTreeWidget* tree);

		// Обработчик вызываемый при щелчке по объекту дерева
		virtual void	 callTabAction(QWidget* widget, QTreeWidgetItem* item);

		// Определение item в дереве справочников
		virtual QList<QTreeWidgetItem*> getSpravItems();

		// Определение item в дереве Администрирования справочников
		virtual QList<QTreeWidgetItem*> getAdminItems();

		// Вызывается после загрузки всех модулей
		virtual void	 initPlug();

		//-- Вызываются из главной программы для передачи объектов
		//void    setListSprav(ListSpravInterface lspr);
		void    setMainWindow(QMainWindow* m);

		//-- Служебные функции
		QWidget* getSubWindow(QTreeWidgetItem* item);
		QWidget* getSubWindow(QString sprav);

		QTreeWidgetItem* getItem(QString sprav);

		void	setWidgetName(QTreeWidgetItem* item, QString wname);
		QString getWidgetName(QTreeWidgetItem* item);
		void	setTabWidget(QTabWidget* tab);
		void	addTabWidget(QWidget* widget, QString text);

	public slots:
		// Контестное меню на дереве справочников
		virtual void contextMenu(QContextMenuEvent* event);

};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(SpravInterface, "com.Roand.ProfiMax.SpravInterface/1.0")
QT_END_NAMESPACE

#endif
