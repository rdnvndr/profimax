#include "plug_admin.h"

#include <math.h>
#include <stdlib.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>

#include "dialoguserpasswd.h"
#include "tablesqlwidget.h"

AdminPlugin::AdminPlugin()
{
	ModulName = tr("Администрирование");
	ModulIcon = QIcon(tr(":icon/icons/users.png"));
}

QWidget* AdminPlugin::getSubWindowAll(QString sprav)
{

	if (sprav == tr("users")) {
		TableSqlWidget* tableSql = new TableSqlWidget();
		tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);
		tableSql->setTable(tr("prof_user_view"));
		tableSql->setFilter("user_id != '00000000-0000-0000-0000-000000000000'");
		tableSql->select();

		QStringList headName;
		headName << "lastname" << "firstname" << "midname" << "login" << "passwd";
		tableSql->setSortColumn(headName);

		tableSql->setHeader("lastname", tr("Фамилия"));
		tableSql->setHeader("firstname", tr("Имя"));
		tableSql->setHeader("midname", tr("Отчество"));
		tableSql->setHeader("login", tr("Логин"));
		tableSql->setHeader("passwd", tr("Пароль"));


		tableSql->show();
		/*
		  QObject::connect(tableSql->toolButtonEdit, SIGNAL(clicked()),
			 this, SLOT(editDialogSotr()));

		  QObject::connect(tableSql->toolButtonInsert, SIGNAL(clicked()),
			 this, SLOT(editDialogSotr()));
		*/

		return tableSql;
	}

	if (sprav == tr("groups")) {
		return NULL;
	}

	if (sprav == tr("import")) {

		QString fileName = QFileDialog::getOpenFileName(tabWidgetMDI , tr("Импорт"), "", tr("SQL Files (*.sql)"));
		if (fileName == tr(""))
			return NULL;

		QProcess builder;

		builder.setProcessChannelMode(QProcess::MergedChannels);
		builder.setStandardInputFile(fileName);
		if (db->driverName() == QString("QMYSQL")) {
			DialogUserPasswd* userPasswd = new DialogUserPasswd(tabWidgetMDI);
			if (userPasswd->exec() != QDialog::Accepted) {
				delete userPasswd;
				return NULL;
			}
			builder.start("mysql", QStringList()
						  << "--user=" + userPasswd->lineEditUser->text()
						  << "--password=" + userPasswd->lineEditPasswd->text()
						  << "--host=" + db->hostName()
						  << db->databaseName()
						 );
		}
		else if (db->driverName() == QString("QPSQL")) {
			builder.start("pg_restore", QStringList()
						  << "--dbname=" + db->databaseName()
						  << "--no-owner"
						  << "--format=custom"
						  << "--file=" + fileName
						 );
		}
		else {
			return NULL;
		}

		if (!builder.waitForFinished()) {
			QMessageBox msgBox;
			msgBox.setText(tr("Ошибка импорта базы данных."));
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setDetailedText(builder.errorString().toUtf8());
			msgBox.exec();
		}
		else {
			QMessageBox msgBox;
			msgBox.setText(tr("Импорт базы данных завершен."));
			QString Err = tr(builder.readAll());
			if (Err == tr("")) {
				msgBox.setText(tr("Импорт базы данных завершен."));
				msgBox.setIcon(QMessageBox::Information);
			}
			else {
				msgBox.setText(tr("Ошибка импорта базы данных."));
				msgBox.setIcon(QMessageBox::Critical);
			}
			msgBox.setDetailedText(Err);
			msgBox.exec();
		}
		return NULL;
	}

	if (sprav == tr("export")) {
		QString fileName = QFileDialog:: getSaveFileName(tabWidgetMDI, tr("Экспорт"), "", tr("SQL Files (*.sql)"));

		if (fileName == tr(""))
			return NULL;



		QProcess builder;
		builder.setProcessChannelMode(QProcess::MergedChannels);
		//builder.setStandardOutputFile (fileName);

		if (db->driverName() == QString("QMYSQL")) {
			DialogUserPasswd* userPasswd = new DialogUserPasswd(tabWidgetMDI);
			if (userPasswd->exec() != QDialog::Accepted) {
				delete userPasswd;
				return NULL;
			}
			builder.start("mysqldump", QStringList() << "--replace"
						  << "--user=" + userPasswd->lineEditUser->text()
						  << "--password=" + userPasswd->lineEditPasswd->text()
						  << "-r" << fileName
						  << "--host=" + db->hostName()
						  << db->databaseName()
						 );
		}
		else if (db->driverName() == QString("QPSQL")) {
			builder.start("pg_dump", QStringList()
						  << "--format=custom"
						  << "--blobs"
						  << "--file=" + fileName
						  << db->databaseName()
						 );
		}
		else {
			return NULL;
		}



		if (!builder.waitForFinished()) {
			QMessageBox msgBox;
			msgBox.setText(tr("Ошибка экспорте базы данных."));
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setDetailedText(builder.errorString().toUtf8());
			msgBox.exec();
		}
		else {
			QMessageBox msgBox;
			QString Err = tr(builder.readAll());
			if (Err == tr("")) {
				msgBox.setText(tr("Экспорт базы данных завершен."));
				msgBox.setIcon(QMessageBox::Information);
			}
			else {
				msgBox.setText(tr("Ошибка экспорт базы данных."));
				msgBox.setIcon(QMessageBox::Critical);
			}
			msgBox.setDetailedText(Err);
			msgBox.exec();
		}
		return NULL;
	}

	if (sprav != tr("")) {
		foreach(SpravInterface * iSprav, *listSprav) {
			QWidget* widget = iSprav->getSubWindow(sprav);
			if (widget != NULL)
				return widget;
		}
	}


	return NULL;
}

void AdminPlugin::setRItems(QTreeWidgetItem* item, SpravInterface * iSprav){
	if (item->childCount() > 0)
		for (int i=0;i<item->childCount();i++){
			QTreeWidgetItem * itemAdmin = item->child(i);
			setWidgetName(itemAdmin, iSprav->getWidgetName(itemAdmin));
			item->addChild(itemAdmin);
			setRItems(itemAdmin,iSprav);
		}
	return;
}

QList<QTreeWidgetItem*> AdminPlugin::getSpravItems()
{
	QList<QTreeWidgetItem*> items;

	QTreeWidgetItem* item = new QTreeWidgetItem();
	QFont font = item->font(0);
	font.setBold(true);
	item->setFont(0, font);
	item->setText(0, tr("Авторизация"));
	items.append(item);

	QTreeWidgetItem* item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/users.png")));
	item1->setText(0, tr("Пользователи"));
	setWidgetName(item1, tr("users"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/groups.png")));
	item1->setText(0, tr("Группы"));
	setWidgetName(item1, tr("groups"));

	item = new QTreeWidgetItem();
	item->setFont(0, font);
	item->setText(0, tr("Работа с данными"));
	items.append(item);

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/import.png")));
	item1->setText(0, tr("Импорт"));
	setWidgetName(item1, tr("import"));

	item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":icon/icons/export.png")));
	item1->setText(0, tr("Экспорт"));
	setWidgetName(item1, tr("export"));

	foreach(SpravInterface * iSprav, *listSprav) {
		if (iSprav->getAdminItems().count() > 0) {
			font.setBold(true);
			item = new QTreeWidgetItem();
			item->setFont(0, font);
			item->setText(0, iSprav->ModulName);
			items.append(item);
			foreach(QTreeWidgetItem * itemAdmin, iSprav->getAdminItems()) {
				setWidgetName(itemAdmin, iSprav->getWidgetName(itemAdmin));
				item->addChild(itemAdmin);
				setRItems(itemAdmin,iSprav);
			}
		}

	}

	return items;
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(plug_Admin, AdminPlugin)
#endif
