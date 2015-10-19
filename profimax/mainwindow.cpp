/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "mainwindow.h"
#include "interfaces.h"

#include <QPluginLoader>
#include <QMessageBox>
#include <QDateEdit>


MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt)
{

	setupUi(this);

	settings = new QSettings("Roand", "ProfiMAX");

	MyTreeWidget* treeWidgetMain = new MyTreeWidget();
	treeWidgetMain->iSprav = NULL;
	treeWidgetMain->setObjectName(QString("treeWidgetMain"));
	treeWidgetMain->setIconSize(QSize(20, 20));
	treeWidgetMain->setFrameStyle(QFrame::NoFrame);
	treeWidgetMain->setHeaderHidden(true);
	treeWidgetMain->setEditTriggers(QAbstractItemView::NoEditTriggers);
	treeWidgetMain->setIndentation(10);
	treeWidgetMain->setStyleSheet("QTreeWidget {background: palette(Window);}");
	connect(treeWidgetMain, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
			this, SLOT(callFuncTreeMain(QTreeWidgetItem*, int)));


	QToolBoxPageButton* toolBoxPageWelcome = new QToolBoxPageButton();
	QTreeWidgetItem* item = new QTreeWidgetItem(treeWidgetMain);
	QFont font = item->font(0);
	font.setBold(true);
	item->setFont(0, font);
	item->setText(0, tr("Общее"));

	QTreeWidgetItem* item1 = new QTreeWidgetItem(item);
	item1->setIcon(0, QIcon(tr(":about/icons/testbed_protocol.png")));
	item1->setText(0, tr("Приветствие"));

	treeWidgetMain->expandAll();
	toolBoxPageWelcome->toolBoxPageLayout->addWidget(treeWidgetMain);
	toolBoxSprav->addWidget(toolBoxPageWelcome);
	connect(toolBoxPageWelcome->lineEditFilter, SIGNAL(textChanged(QString)), treeWidgetMain, SLOT(searchShowAllItem(QString)));



    connect(actionAbout,   SIGNAL(triggered()), this, SLOT(onActionAbout()));
    connect(actionHelp,   SIGNAL(triggered()), this, SLOT(onActionHelp()));
    connect(actionPref,   SIGNAL(triggered()), this, SLOT(onActionPref()));
    connect(actionConnect, SIGNAL(triggered()), this, SLOT(onActionConnect()));

	connect(tabWidgetMDI,  SIGNAL(tabCloseRequested(int)),
			this, SLOT(closeTab(int)));

	connect(actionCut,    SIGNAL(triggered()), this, SLOT(onCut()));
	connect(actionCopy,   SIGNAL(triggered()), this, SLOT(onCopy()));
	connect(actionPaste,  SIGNAL(triggered()), this, SLOT(onPaste()));
	connect(actionUndo,   SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(actionRedo,   SIGNAL(triggered()), this, SLOT(onRedo()));
	connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(onSelectAll()));

	connect(actionPrint, SIGNAL(triggered()), this, SLOT(onActionPrint()));
	connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(onActionPrintPreview()));
	connect(actionExportPDF, SIGNAL(triggered()), this, SLOT(onActionExportPDF()));
	connect(actionSaveReport, SIGNAL(triggered()), this, SLOT(onActionSaveReport()));
	connect(actionOpenReport, SIGNAL(triggered()), this, SLOT(onActionOpenReport()));

	connect(actionUpdate, SIGNAL(triggered()), this, SLOT(onActionUpdate()));

	connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)),
			this, SLOT(onFocusChanged(QWidget*, QWidget*)));

	connect(tabWidgetMDI, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

}


void MainWindow::closeEvent(QCloseEvent* event)
{
	writeSettings();
	event->accept();
}

void MainWindow::cleanFileUpdate(QString direct)
{

	QDir dir(direct);

	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); i++) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName() != QString(".") && fileInfo.fileName() != QString("..")) {
			if (fileInfo.isDir()) {
				cleanFileUpdate(dir.absolutePath() + QString("/") + fileInfo.fileName());
			}
			else {

				if (fileInfo.fileName().mid(0, 4) == QString("old_")) {

					QFile fileNameOld(dir.absolutePath() + QString("/") + fileInfo.fileName());
					fileNameOld.remove();
				}
			}
		}
	}
}

void MainWindow::readSettings()
{

	settings->beginGroup("MainWindow");
	resize(settings->value("size", QSize(687, 582)).toSize());
	move(settings->value("pos", QPoint(200, 200)).toPoint());

	settings->endGroup();

	settings->beginGroup("Update");

	if (settings->value("update", false).toBool()) {
		settings->setValue("update", false);
		cleanFileUpdate(QString(""));
	}
	settings->endGroup();
}

void MainWindow::writeSettings()
{

	settings->beginGroup("MainWindow");
	settings->setValue("size", size());
	settings->setValue("pos", pos());

	settings->endGroup();

}

void MainWindow::onEditOpenReport()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		row = 0;

	QString reportid = tableSql->model->record(row).value("report_id").toString();
	QString fileform = tableSql->model->record(row).value("fileform").toString();
	QString reportname = tableSql->model->record(row).value("name").toString();
	QString timeshtamp = tableSql->model->record(row).value("sdate").toDateTime().toString("dd-MM-yyyy HH:mm");

	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (QString("%1 (%2)").arg(reportname).arg(timeshtamp) == (tabWidgetMDI->tabText(i))) {
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	QStringList param;
	QSqlQuery query;

	if (query.exec(QString("SELECT * FROM prof_repparam WHERE report_id='%1'").arg(reportid))) {
		while (query.next())
			for (int i = 0; i < query.record().count(); i++) {
				param << query.record().value(query.record().indexOf("name")).toString()
					  << query.record().value(query.record().indexOf("value")).toString();
			}
		ProfimaxReport* report = new ProfimaxReport();
		report->setReportName(reportname);
		report->setInsertParam(param);

		if (!report->load(fileform))
			QMessageBox::critical(0, "", tr("Отчет не загрузился"));

		int index = tabWidgetMDI->addTab(report, QString("%1 (%2)").arg(reportname).arg(timeshtamp));
		tabWidgetMDI->setCurrentIndex(index);
	}
}

void MainWindow::onActionOpenReport()
{
	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (tr("Сохраненные отчеты") == (tabWidgetMDI->tabText(i))) {
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	TableSqlWidget* tableSql = new TableSqlWidget();
	tableSql->setTable(tr("prof_report"));

	tableSql->setEditStrategy(QSqlTableModel::OnFieldChange);
	tableSql->select();

	QStringList headName;
	headName << "sdate" << "name";
	tableSql->setSortColumn(headName);
	tableSql->setHeader("name", tr("Наименование отчета"));
	tableSql->setHeader("sdate", tr("Время создания"));

	tableSql->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableSql->toolButtonInsert->hide();
	tableSql->toolButtonDelete->hide();

	QObject::connect(tableSql->toolButtonEdit, SIGNAL(clicked()),
					 this, SLOT(onEditOpenReport()));

	//tableSql->model->setSort(tableSql->model->fieldIndex("sdate"),Qt::AscendingOrder);
	tableSql->tableViewSql->sortByColumn(tableSql->model->fieldIndex("sdate"), Qt::DescendingOrder);
	tableSql->show();

	int index = tabWidgetMDI->addTab(tableSql, tr("Сохраненные отчеты"));
	tabWidgetMDI->setCurrentIndex(index);
}

void MainWindow::onActionSaveReport()
{

	ProfimaxReport* report = qobject_cast<ProfimaxReport*>(tabWidgetMDI->currentWidget());
	if (report != NULL) {
		QStringList param = report->getInsertParam();
		QSqlQuery query;

		QString sqlstr = QString("INSERT INTO prof_report (name,fileform) VALUES ('%1','%2')").arg(report->getReportName()).arg(report->fileform);

		if (query.driver()->handle().typeName() == "PGconn*")
			sqlstr = sqlstr + QString(" RETURNING report_id");
		if (query.exec(sqlstr)) {
			QString UUID;
			if (query.driver()->handle().typeName() != "MYSQL*")
				query.exec(QString("SELECT @lastuuid"));
			while (query.next())
				UUID = query.record().value(0).toString();

			for (int i = 0; i < param.count(); i = i + 2) {
				query.exec(QString("INSERT INTO prof_repparam (name,value,report_id) VALUES ('%2','%3','%4')").arg(param.at(i)).arg(param.at(i + 1)).arg(UUID));
			}
			QMessageBox::information(this, tr("Информация"), tr("Отчет успешно сохранен:"));
		}
		else {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при сохранении отчета\n") + query.lastError().text());
		}
	}
	else {
		QMessageBox::critical(this, tr("Ошибка"), tr("Нет активного отчета для сохранения"));
	}
}

void MainWindow::onCut()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void MainWindow::onCopy()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void MainWindow::onPaste()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void MainWindow::onUndo()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_Z, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void MainWindow::onRedo()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_Y, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void MainWindow::onSelectAll()
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)
	   ) {
		QKeyEvent event(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

// Загрузка модуля
void MainWindow::loadPlugins()
{
	QToolBar* toolbar = new QToolBar();
	dockWidgetContentsSprav->layout()->addWidget(toolbar);
	toolbar->setIconSize(QSize(20, 20));
	toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolbar->show();

	QAction* actionPlugMain = toolbar->addAction(QIcon(":/about/icons/Startup Wizard.png"), tr("Главная"));
	actionPlugMain->setObjectName(QString("actionPlugMain-0"));
	connect(actionPlugMain, SIGNAL(triggered()), this, SLOT(onActionPlug()));

	QDir pluginsDir = QDir(qApp->applicationDirPath());
	if (!pluginsDir.cd("plugins")) {
		QMessageBox::critical(this, tr("Ошибка"), tr("Каталог с модулями не найден"));
	}

	int i = 1;

	foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

		QObject* plugin = loader.instance();
		if (plugin) {
			SpravInterface* iSprav = qobject_cast<SpravInterface*>(plugin);
			// Создание кнопки
			QAction* action = toolbar->addAction(iSprav->ModulIcon, iSprav->ModulName);
			action->setObjectName(QString("ActionPlugin-%1").arg(i));
			connect(action, SIGNAL(triggered()), this, SLOT(onActionPlug()));
			i++;

			listSprav.append(iSprav);

		}
	}

	foreach (SpravInterface* iSprav,listSprav){
		createPlugObject(iSprav);
		//iSprav->setListSprav(listSprav);
	}

	showTabWelcome();
	//toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}


bool MainWindow::onActionConnect()
{
	DialogConnect* windowConnect = new DialogConnect(this);

	settings->beginGroup("DialogConnect");
	windowConnect->comboDriver->setCurrentIndex(settings->value("driver").toInt());
	windowConnect->editDatabase->setText(settings->value("database").toString());
	windowConnect->editHostname->setText(settings->value("hostname").toString());
	windowConnect->portSpinBox->setValue(settings->value("port").toInt());
	windowConnect->editUsername->setText(settings->value("username").toString());
	settings->endGroup();

	while (true) {
		if (windowConnect->exec() == QDialog::Accepted) {
			if (db.isOpen()) {
				closeTabAll();
				db.close();
				db.removeDatabase(db.databaseName());
			}

			db = QSqlDatabase::addDatabase(windowConnect->comboDriver->currentText());
			db.setDatabaseName(windowConnect->editDatabase->text());
			db.setHostName(windowConnect->editHostname->text());
			db.setPort(windowConnect->portSpinBox->value());

			if (!db.open(windowConnect->editUsername->text(), windowConnect->editPassword->text())) {
				QSqlError err = db.lastError();
				if (err.type() != QSqlError::NoError)
					QMessageBox::warning(this, tr("Не удается открыть базу данных"), tr("Произошла ошибка при "
										 "создании соединения: ") + err.text());
			}
			else {
				settings->beginGroup("DialogConnect");
				settings->setValue("driver", windowConnect->comboDriver->currentIndex());
				settings->setValue("database", windowConnect->editDatabase->text());
				settings->setValue("hostname", windowConnect->editHostname->text());
				settings->setValue("port", windowConnect->portSpinBox->value());
				settings->setValue("username", windowConnect->editUsername->text());
				settings->endGroup();
				break;
			}
		}
		else {
			delete windowConnect;
			return false;
		}
	}
	delete windowConnect;
	return true;
}

void MainWindow::onActionAbout()
{
	DialogAbout* windowAbout = new DialogAbout(this);
	windowAbout->exec();
	delete windowAbout;

}

void MainWindow::onActionHelp()
{
	QProcess* process = new QProcess(this);
	QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath)
				  + QLatin1String("/assistant");

	QStringList args;
	args << QLatin1String("-collectionFile")
		 /*<< QLibraryInfo::location(QLibraryInfo::ExamplesPath)
			   + QLatin1String("/help/simpletextviewer/documentation/simpletextviewer.qhc")*/
		 << QLatin1String("help/profimax.qhc")
		 << QLatin1String("-enableRemoteControl");


	process->start(app, args);
	if (!process->waitForStarted()) {
		QMessageBox::critical(this, tr("Remote Control"),
							  tr("Could not start Qt Assistant from %1.").arg(app));
		return;
	}

	// show index page
	QTextStream str(process);
	str << QLatin1String("SetSource qthelp://com.roand.profimax/doc/index.html")
		<< QLatin1Char('\0') << endl;
}

void MainWindow::onActionPref()
{

	DialogPref* windowPref = new DialogPref(this);

	settings->beginGroup("Update");
	windowPref->lineEditSrv->setText(settings->value("Srv", "").toString());
	windowPref->comboBoxMode->setCurrentIndex(settings->value("ModeSrv", 0).toInt());
	windowPref->lineEditUserSrv->setText(settings->value("UserSrv", "").toString());
	windowPref->lineEditPasswdSrv->setText(settings->value("PasswdSrv", "").toString());

	windowPref->groupBoxProxy->setChecked(settings->value("proxy", false).toBool());
	windowPref->lineEditProxySrv->setText(settings->value("SrvProxy", "").toString());
	windowPref->spinBoxProxyPort->setValue(settings->value("ProxyPort", 0).toInt());
	windowPref->lineEditProxyUser->setText(settings->value("ProxyUser", "").toString());
	windowPref->lineEditProxyPasswd->setText(settings->value("ProxyPasswd", "").toString());
	settings->endGroup();

	if (windowPref->exec() == QDialog::Accepted) {
		settings->beginGroup("Update");
		settings->setValue("Srv", windowPref->lineEditSrv->text());
		settings->setValue("Srv", windowPref->lineEditSrv->text());
		settings->setValue("ModeSrv", windowPref->comboBoxMode->currentIndex());
		settings->setValue("UserSrv", windowPref->lineEditUserSrv->text());
		settings->setValue("PasswdSrv", windowPref->lineEditPasswdSrv->text());

		settings->setValue("proxy", windowPref->groupBoxProxy->isChecked());
		settings->setValue("SrvProxy", windowPref->lineEditProxySrv->text());
		settings->setValue("ProxyPort", windowPref->spinBoxProxyPort->value());
		settings->setValue("ProxyUser", windowPref->lineEditProxyUser->text());
		settings->setValue("ProxyPasswd", windowPref->lineEditProxyPasswd->text());
		settings->endGroup();
	}

	delete windowPref;

}

void MainWindow::showTabWelcome()
{

	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (tr("MainWelcome") == (tabWidgetMDI->widget(i)->objectName())) {
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	WelcomeWidget* welcomewidget = new WelcomeWidget(this);
	welcomewidget->restoreButton();
	welcomewidget->setObjectName(tr("MainWelcome"));
	int index = tabWidgetMDI->addTab(welcomewidget, tr("Добро пожаловать"));
	tabWidgetMDI->setCurrentIndex(index);
}

void MainWindow::closeTabAll()
{
	for (int i = tabWidgetMDI->count() - 1; i >= 0; i--)
		if (tr("MainWelcome") != (tabWidgetMDI->widget(i)->objectName()))
			tabWidgetMDI->removeTab(i);
	showTabWelcome();
}

void MainWindow::closeTab(int index)
{

	if (tr("MainWelcome") == (tabWidgetMDI->widget(index)->objectName()) && (tabWidgetMDI->count() == 1))
		return;

	tabWidgetMDI->removeTab(index);

	if (tabWidgetMDI->count() == 0)
		showTabWelcome();
}

// Вызов функции плагина
void MainWindow::callFuncAction()
{

	WelcomeToolButton* action = qobject_cast<WelcomeToolButton*>(sender());
	QString objectName = action->objectName().split(".").at(1);
	SpravInterface* iSprav = action->iSprav;
	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (objectName.split("-").at(0) == (tabWidgetMDI->widget(i)->objectName())) {
			iSprav->callTabAction(tabWidgetMDI->widget(i), iSprav->getItem(objectName));
			//QMessageBox::critical(NULL,tr("Ошибка"),objectName);
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	QWidget* tabwidget = iSprav->getSubWindow(objectName);

	if (tabwidget != NULL) {

		int index = tabWidgetMDI->addTab(tabwidget, iSprav->getItem(objectName.split("-").at(0))->text(0));
		iSprav->callTabAction(tabwidget, iSprav->getItem(objectName));
		tabWidgetMDI->setCurrentIndex(index);
	}
}

// Создание меню справочников
void MainWindow::createPlugObject(QObject* plugin)
{
	SpravInterface* iSprav = qobject_cast<SpravInterface*>(plugin);

	if (iSprav) {
		iSprav->db = &this->db;
		iSprav->listSprav = &this->listSprav;

		iSprav->setTabWidget(this->tabWidgetMDI);
		iSprav->setMainWindow(this);
		iSprav->initPlug();

		// Создание toolbar для модулей
		QToolBoxPageButton* toolBoxPageButton = new QToolBoxPageButton();

		toolBoxSprav->addWidget(toolBoxPageButton);
		addToToolBox(plugin, toolBoxPageButton->toolBoxPageLayout);
	}

}

// Добавление справочников в toolBox
void MainWindow::addToToolBox(QObject* plugin, QVBoxLayout* box)
{

	MyTreeWidget* tree = new MyTreeWidget();

	tree->iSprav = qobject_cast<SpravInterface*>(plugin);
	tree->setObjectName(QString("treeWidgetModul") + tree->iSprav->ModulName);
	tree->setIconSize(QSize(20, 20));
	tree->setFrameStyle(QFrame::NoFrame);
	tree->setHeaderHidden(true);
	tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tree->setIndentation(10);
	tree->setStyleSheet("QTreeWidget {background: palette(Window);}");

	connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
			this, SLOT(callFuncTreeItem(QTreeWidgetItem* , int)));

	connect(tree, SIGNAL(contextMenuSignal(QContextMenuEvent*)), tree->iSprav, SLOT(contextMenu(QContextMenuEvent*)));

	QList<QTreeWidgetItem*> treeitem = tree->iSprav->getSpravItems();
	if (treeitem.count() > 0)
		tree->insertTopLevelItems(0, treeitem);
	tree->expandAll();
	box->addWidget(tree);
	QToolBoxPageButton* pagebox = qobject_cast<QToolBoxPageButton*>(box->parentWidget());
	connect(pagebox->lineEditFilter, SIGNAL(textChanged(QString)), tree, SLOT(searchShowAllItem(QString)));

}

void MainWindow::onFocusChanged(QWidget* old, QWidget* now)
{
	if ((qobject_cast<QLineEdit*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QSpinBox*>(QApplication::focusWidget()) != NULL) ||
		(qobject_cast<QDateEdit*>(QApplication::focusWidget()) != NULL)) {

		actionCut->setEnabled(true);
		actionCopy->setEnabled(true);
		actionPaste->setEnabled(true);
		actionRedo->setEnabled(true);
		actionUndo->setEnabled(true);
		actionSelectAll->setEnabled(true);

	}
	else {
		actionCut->setDisabled(true);
		actionCopy->setDisabled(true);
		actionPaste->setDisabled(true);
		actionRedo->setDisabled(true);
		actionUndo->setDisabled(true);
		actionSelectAll->setDisabled(true);
	}
}

void MainWindow::onActionPrint()
{
	ProfimaxReport* report = qobject_cast<ProfimaxReport*>(tabWidgetMDI->currentWidget());
	if (report != NULL) {
		report->filePrint();
	}
	else {
		QMessageBox::critical(this, tr("Ошибка"), tr("Возникли проблемы при печати отчета"));
	}
}

void MainWindow::onActionPrintPreview()
{

	ProfimaxReport* report = qobject_cast<ProfimaxReport*>(tabWidgetMDI->currentWidget());
	if (report != NULL) {
		report->filePrintPreview();
	}
	else {
		QMessageBox::critical(this, tr("Ошибка"), tr("Возникли проблемы при печати отчета"));
	}
}

void MainWindow::onActionExportPDF()
{
	ProfimaxReport* report = qobject_cast<ProfimaxReport*>(tabWidgetMDI->currentWidget());
	if (report != NULL) {
		report->filePrintPdf();
	}
	else {
		QMessageBox::critical(this, tr("Ошибка"), tr("Возникли проблемы при печати отчета"));
	}
}

void MainWindow::ftpDownDir(UpdateFTP* updateFtp, QString direct, QString ftpDir)
{

	QDir dir(direct);

	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); i++) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName() != QString(".") && fileInfo.fileName() != QString("..")) {
			if (fileInfo.isDir()) {
				ftpDownDir(updateFtp, dir.absolutePath() + QString("/") + fileInfo.fileName(), ftpDir + QString("/") + fileInfo.fileName());
			}
			else {
				updateFtp->dfile << ftpDir + QString("/") + fileInfo.fileName();
			}
		}
	}
}

void MainWindow::onActionUpdate()
{

	settings->beginGroup("Update");

#if defined(Q_OS_WIN)
	QUrl url("ftp://" + settings->value("Srv", "").toString() + "/win32");
#elif defined(Q_OS_UNIX)
	QUrl url("ftp://" + settings->value("Srv", "").toString() + "/test");
#endif

	url.setUserName(settings->value("UserSrv", "").toString());
	url.setPassword(settings->value("PasswdSrv", "").toString());

	if (settings->value("proxy", false).toBool()) {
		QNetworkProxy proxy;
		proxy.setType(QNetworkProxy::FtpCachingProxy);
		proxy.setHostName(settings->value("SrvProxy", "").toString());
		proxy.setPort(settings->value("ProxyPort", 0).toInt());
		proxy.setUser(settings->value("ProxyUser", "").toString());
		proxy.setPassword(settings->value("ProxyPasswd", "").toString());
		QNetworkProxy::setApplicationProxy(proxy);
	}


	UpdateFTP* updateFtp = new UpdateFTP(this);

	if (settings->value("ModeSrv", 0).toInt() == 0)
		updateFtp->ftpConnect(url, true);
	else
		updateFtp->ftpConnect(url, false);

	settings->endGroup();

	ftpDownDir(updateFtp, QString(""), QString(""));
	updateFtp->start();
}

void MainWindow::currentChanged(int index)
{

	if (qobject_cast<ProfimaxReport*>(tabWidgetMDI->currentWidget()) != NULL) {
		actionPrint->setEnabled(true);
		actionPrintPreview->setEnabled(true);
		actionExportPDF->setEnabled(true);
		actionSaveReport->setEnabled(true);

	}
	else {
		actionPrint->setDisabled(true);
		actionPrintPreview->setDisabled(true);
		actionExportPDF->setDisabled(true);
		actionSaveReport->setDisabled(true);
	}
}

// Вызов функции из дерева
void MainWindow::callFuncTreeItem(QTreeWidgetItem* item, int column)
{

	MyTreeWidget* tree = qobject_cast<MyTreeWidget*>(item->treeWidget());

	for (int i = 0; i < tabWidgetMDI->count(); i++)
		if (tree->iSprav->getWidgetName(item).split("-").at(0) == (tabWidgetMDI->widget(i)->objectName())) {
			tree->iSprav->callTabAction(tabWidgetMDI->widget(i), item);
			tabWidgetMDI->setCurrentIndex(i);
			return;
		}

	QWidget* tabwidget = tree->iSprav->getSubWindow(item);

	if (tabwidget != NULL) {
		int index = tabWidgetMDI->addTab(tabwidget, tree->iSprav->getItem(tree->iSprav->getWidgetName(item).split("-").at(0))->text(column));
		tree->iSprav->callTabAction(tabwidget, item);
		tabWidgetMDI->setCurrentIndex(index);
	}
}

// Вызов функции из главного дерева
void MainWindow::callFuncTreeMain(QTreeWidgetItem* item, int column)
{

	if (item->text(0) == tr("Приветствие"))
		showTabWelcome();
}
// Вызов вкладки плагина
void MainWindow::onActionPlug()
{
	QAction* action = qobject_cast<QAction*>(sender());
	int index = action->objectName().split("-").at(1).toInt();
	dockWidgetSprav->setWindowTitle(action->text());

	toolBoxSprav->setCurrentIndex(index);
}
