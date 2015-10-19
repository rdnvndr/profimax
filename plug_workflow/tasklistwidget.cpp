#include <QMessageBox>

#include "tasklistwidget.h"
#include "texteditwidget.h"
#include "dialogtaskstart.h"
#include "mailsqlrelationaldelegate.h"
#include "dialogforward.h"

TaskListWidget::TaskListWidget(QWidget* pwgt) : QWidget(pwgt)
{
	setupUi(this);
	flagbutton = false;

	//textEdit->layout()->setMargin(0);
	tableMailView->layout()->setMargin(0);
	tableMailView->tableViewSql->verticalHeader()->hide();
	tableMailView->tableViewSql->setShowGrid(false);
	tableMailView->tableViewSql->horizontalHeader()->setCascadingSectionResizes(true);
	tableMailView->tableViewSql->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
	tableMailView->tableViewSql->horizontalHeader()->setStretchLastSection(true);
	tableMailView->tableViewSql->verticalHeader()->setDefaultSectionSize(20);

	//tableViewSql->setSelectionMode(QAbstractItemView::ContiguousSelection);
	tableMailView->tableViewSql->setSelectionBehavior(QAbstractItemView::SelectRows);

	// Установить только для чтения
	tableMailView->tableViewSql->setEditTriggers(QAbstractItemView::NoEditTriggers);
	textEdit->setReadOnly(true);

	// Разделитель
	lineToolView = new QFrame(tableMailView);
	lineToolView->setObjectName(QString::fromUtf8("lineToolViewTask"));
	lineToolView->setFrameShape(QFrame::VLine);
	lineToolView->setFrameShadow(QFrame::Sunken);
	tableMailView->horizontalLayoutViewTool->insertWidget(tableMailView->horizontalLayoutViewTool->count() - 1, lineToolView);

	// Добавление кнопки свойства задачи
	toolButtonProp = new QToolButton(tableMailView);
	toolButtonProp->setObjectName(QString::fromUtf8("toolButtonProp"));
	QIcon* icon = new QIcon();
	icon->addFile(QString::fromUtf8(":/icon/icons/proptask.png"), QSize(), QIcon::Normal, QIcon::Off);
	toolButtonProp->setIcon(*icon);
	toolButtonProp->setIconSize(QSize(20, 20));
	toolButtonProp->setAutoRaise(true);
	toolButtonProp->setToolTip(tr("Свойства задачи"));
	tableMailView->horizontalLayoutViewTool->insertWidget(tableMailView->horizontalLayoutViewTool->count() - 1, toolButtonProp);
	QObject::connect(toolButtonProp, SIGNAL(clicked()), this, SLOT(onclickButtonProp()));

	// Добавление кнопки перепоручения
	toolButtonForward = new QToolButton(tableMailView);
	toolButtonForward->setObjectName(QString::fromUtf8("toolButtonForward"));
	icon = new QIcon();
	icon->addFile(QString::fromUtf8(":/icon/icons/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
	toolButtonForward->setIcon(*icon);
	toolButtonForward->setIconSize(QSize(20, 20));
	toolButtonForward->setAutoRaise(true);
	toolButtonForward->setToolTip(tr("Перепоручить"));
	tableMailView->horizontalLayoutViewTool->insertWidget(tableMailView->horizontalLayoutViewTool->count() - 1, toolButtonForward);
	QObject::connect(toolButtonForward, SIGNAL(clicked()), this, SLOT(onclickButtonForward()));

	// Редактировать сообщение
	QObject::connect(tableMailView->toolButtonEdit, SIGNAL(clicked()),
					 this, SLOT(editDialogMsg()));

	// Создание сообщения
	tableMailView->disconnectButtonInsert();
	QObject::connect(tableMailView->toolButtonInsert, SIGNAL(clicked()),
					 this, SLOT(addDialogMsg()));

	// Удалить сообщение
	tableMailView->disconnectButtonDelete();
	QObject::connect(tableMailView->toolButtonDelete, SIGNAL(clicked()),
					 this, SLOT(deleteDialogMsg()));

	// Обновить сообщение
	tableMailView->disconnectButtonRefresh();
	QObject::connect(tableMailView->toolButtonRefresh, SIGNAL(clicked()),
					 this, SLOT(onclickButtonRefresh()));

	tableMailView->setEditStrategy(QSqlTableModel::OnFieldChange);

}

void TaskListWidget::setTable(const QString& tableName)
{
	tableMailView->setTable(tableName);

	QObject::connect(tableMailView->tableViewSql->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
					 this, SLOT(showMsgMail(QModelIndex, QModelIndex)));

	// Выбор строчки и изменение статуса кнопок
	QObject::connect(tableMailView->tableViewSql->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
					 this, SLOT(setStatButton(QModelIndex, QModelIndex)));

}

void TaskListWidget::setHeader(const QString& col, const QString& colname)
{
	tableMailView->setHeader(col, colname);
}

// Установить связь поля с полем другой таблицы
void TaskListWidget::setRelation(QString mycol, QString table, QString linkcol, QString showcol)
{
	tableMailView->setRelation(mycol, table, linkcol, showcol);
}

void TaskListWidget::setSortColumn(QStringList stl)
{
	tableMailView->setSortColumn(stl);
}

void TaskListWidget::setFieldNameBool(QStringList Name)
{
	tableMailView->setFieldNameBool(Name);
}

void TaskListWidget::select()
{
	tableMailView->select();
	ProfimaxDelegate* mysqlrel = qobject_cast<ProfimaxDelegate*> (tableMailView->tableViewSql->itemDelegate());
	MailSqlRelationalDelegate* mailsqlrel = new MailSqlRelationalDelegate();
	mailsqlrel->setFieldName(mysqlrel->fieldName);
	tableMailView->tableViewSql->setItemDelegate(mailsqlrel);
}

void TaskListWidget::setHtml(const QString& text)
{
	textEdit->setHtml(text);
}

void TaskListWidget::createTaskButton()
{

	// Если задача начата то отказаться не возможно
	// Если задача еще не начата то можно отказаться

	flagbutton = true;

	// Добавление кнопки старт
	toolButtonStart = new QToolButton(tableMailView);
	toolButtonStart->setObjectName(QString::fromUtf8("toolButtonStart"));
	QIcon* icon = new QIcon();
	icon->addFile(QString::fromUtf8(":/icon/icons/start.png"), QSize(), QIcon::Normal, QIcon::Off);
	toolButtonStart->setIcon(*icon);
	toolButtonStart->setIconSize(QSize(20, 20));
	toolButtonStart->setAutoRaise(true);
	toolButtonStart->setToolTip(tr("Запустить задачу"));
	tableMailView->horizontalLayoutViewTool->insertWidget(tableMailView->horizontalLayoutViewTool->count() - 1, toolButtonStart);
	QObject::connect(toolButtonStart, SIGNAL(clicked()), this, SLOT(startTask()));

	// Добавление кнопки окончание задачи
	toolButtonFinish = new QToolButton(tableMailView);
	toolButtonFinish->setObjectName(QString::fromUtf8("toolButtonFinish"));
	icon->addFile(QString::fromUtf8(":/icon/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
	toolButtonFinish->setIcon(*icon);
	toolButtonFinish->setIconSize(QSize(20, 20));
	toolButtonFinish->setAutoRaise(true);
	toolButtonFinish->setToolTip(tr("Закончить задачу"));
	tableMailView->horizontalLayoutViewTool->insertWidget(tableMailView->horizontalLayoutViewTool->count() - 1, toolButtonFinish);
	QObject::connect(toolButtonFinish, SIGNAL(clicked()), this, SLOT(finishTask()));

}

void TaskListWidget::editDialogMsg()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {

		// Нужен запрос подтверждения "Вы уверены что хотите удалить сообщение"

		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();

		DialogMsg* WindowMsg = new DialogMsg(this);
		WindowMsg->setModel(tableSql->model);
		WindowMsg->loadMessage(msgid);
		WindowMsg->show();
	}

}

void TaskListWidget::addDialogMsg()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	DialogMsg* WindowMsg = new DialogMsg(this);
	WindowMsg->setModel(tableSql->model);
	WindowMsg->show();

}

void TaskListWidget::deleteDialogMsg()
{

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {

		// Нужен запрос подтверждения "Вы уверены что хотите удалить сообщение"

		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;
		if (!query.exec(QString("DELETE FROM %1 WHERE msg_id = '%2'")
						.arg(tableSql->model->tableName())
						.arg(msgid)))
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при удалении сообщения\n") + query.lastError().text());
		else
			tableSql->onClickToolButtonRefresh();
	}

}

void TaskListWidget::startTask()
{
	// не начатая задача пауза и нет даты начала
	//если не начато то установить что начато и в работе

	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {
		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;

		if (!query.exec(QString("select task_status('%1')").arg(msgid))) {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при получении статуса задачи\n") + query.lastError().text());
			return;
		}
		query.next();
		int task_status = query.value(0).toInt();

		if (task_status == 0) {
			if (!query.exec(QString("INSERT INTO prof_msgtask (msg_id,status_id) VALUES ('%1',3)").arg(msgid)))
				QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
		}
		else {
			if (!query.exec(QString("UPDATE prof_msgtask SET status_id=3 WHERE msg_id='%1'").arg(msgid))) {
				QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
			}
		}
		setStatButton(tableSql->tableViewSql->currentIndex(), tableSql->tableViewSql->currentIndex());

	}
}

void TaskListWidget::stopTask()
{
	// Изменить статус что приостановлено
	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {
		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;

		if (!query.exec(QString("UPDATE prof_msgtask SET status_id=4 WHERE msg_id='%1'").arg(msgid))) {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
		}
		setStatButton(tableSql->tableViewSql->currentIndex(), tableSql->tableViewSql->currentIndex());
	}
}

void TaskListWidget::finishTask()
{
	// если задача не начата то это отказ от нее
	// если задача начата то это завершение задачи
	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {
		DialogTaskStart* dialogTaskStart = new DialogTaskStart(this);
		dialogTaskStart->lineEditStatus->setReadOnly(true);
		dialogTaskStart->lineEditBegin->setReadOnly(true);
		dialogTaskStart->lineEditEnd->setReadOnly(true);
		dialogTaskStart->lineEditTaskBegin->setReadOnly(true);
		dialogTaskStart->lineEditTaskEnd->setReadOnly(true);
		dialogTaskStart->spinBoxProcent->setReadOnly(true);

		QDataWidgetMapper* mappermsg = new QDataWidgetMapper();

		mappermsg->setModel(tableSql->model);

		mappermsg->addMapping(dialogTaskStart->lineEditBegin, tableSql->model->fieldIndex("bdate"));
		mappermsg->addMapping(dialogTaskStart->lineEditEnd, tableSql->model->fieldIndex("edate"));
		mappermsg->addMapping(dialogTaskStart->lineEditTaskBegin, tableSql->model->fieldIndex("btaskdate"));
		mappermsg->addMapping(dialogTaskStart->lineEditTaskEnd, tableSql->model->fieldIndex("etaskdate"));
		mappermsg->addMapping(dialogTaskStart->spinBoxProcent, tableSql->model->fieldIndex("percentage"));
		mappermsg->addMapping(dialogTaskStart->lineEditStatus, tableSql->model->fieldIndex("status_id"));
		dialogTaskStart->textEdit->setHtml(tableSql->model->record(row).value("comm").toString());
		//QMessageBox::critical(0,"",tableSql->model->record(row).value("body").toString());

		mappermsg->setCurrentIndex(row);
		if (dialogTaskStart->exec() == QDialog::Accepted) {

			QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
			QSqlQuery query;

			if (!query.exec(QString("select task_status('%1')").arg(msgid))) {
				QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при получении статуса задачи\n") + query.lastError().text());
				return;
			}
			query.next();
			int task_status = query.value(0).toInt();

			if (task_status == 0) {
				if (!query.exec(QString("INSERT INTO prof_msgtask (msg_id,status_id,comm) VALUES ('%1',2,'%2')").arg(msgid).arg(dialogTaskStart->textEdit->textEdit->toHtml().replace("'", "\""))))
					QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
			}
			else {
				if (!query.exec(QString("UPDATE prof_msgtask SET status_id=5,comm='%2' WHERE msg_id='%1'").arg(msgid).arg(dialogTaskStart->textEdit->textEdit->toHtml().replace("'", "\"")))) {
					QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
				}
			}

			setStatButton(tableSql->tableViewSql->currentIndex(), tableSql->tableViewSql->currentIndex());
		}

		delete dialogTaskStart;
	}
}

void TaskListWidget::refuseTask()
{
	// если задача не начата то это отказ от нее
	// если задача начата то это завершение задачи
	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {
		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;

		if (!query.exec(QString("UPDATE prof_msgtask SET status_id=5 WHERE msg_id='%1'").arg(msgid))) {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении статуса задачи\n") + query.lastError().text());
		}
	}
}

void TaskListWidget::setStatButton(QModelIndex row1, QModelIndex row2)
{
	if (row1.model() == NULL)
		return;
	QString stype = row1.model()->data(row1.sibling(row1.row(), this->tableMailView->model->fieldIndex("type_id"))).toString();

	if (stype.simplified() == tr("Задание")) {

		QString msgid = row1.model()->data(row1.sibling(row1.row(), this->tableMailView->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;
		if (!query.exec(QString("select task_status('%1')").arg(msgid))) {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при получении статуса задачи\n") + query.lastError().text());
			return;
		}
		query.next();
		int task_status = query.value(0).toInt();

		// Включение отключение кнопок
		toolButtonProp->setEnabled(true);
		if (flagbutton == false)
			return;
		if (task_status == 2 or task_status == 5) {
			// Задача закончена или отклонена
			toolButtonStart->setDisabled(true);
			toolButtonFinish->setDisabled(true);
		}
		else {
			toolButtonStart->setEnabled(true);
			toolButtonFinish->setEnabled(true);
		}

		// Изменение старт и пауза
		QIcon* icon = new QIcon();
		switch (task_status) {
			case 3:
				icon->addFile(QString::fromUtf8(":/icon/icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
				toolButtonStart->setToolTip(tr("Приостановить задачу"));
				toolButtonStart->setIcon(*icon);
				toolButtonStart->disconnect(SIGNAL(clicked()));
				QObject::connect(toolButtonStart, SIGNAL(clicked()), this, SLOT(stopTask()));
				break;
			case 4:
				toolButtonStart->disconnect(SIGNAL(clicked()));
				QObject::connect(toolButtonStart, SIGNAL(clicked()), this, SLOT(startTask()));
			default:
				icon->addFile(QString::fromUtf8(":/icon/icons/start.png"), QSize(), QIcon::Normal, QIcon::Off);
				toolButtonStart->setToolTip(tr("Запустить задачу"));
				toolButtonStart->setIcon(*icon);
		}


	}
	else {

		toolButtonProp->setDisabled(true);
		if (flagbutton == false)
			return;

		toolButtonStart->setDisabled(true);
		toolButtonFinish->setDisabled(true);

		QIcon* icon = new QIcon();
		icon->addFile(QString::fromUtf8(":/icon/icons/start.png"), QSize(), QIcon::Normal, QIcon::Off);
		toolButtonStart->setToolTip(tr("Запустить задачу"));
		toolButtonStart->setIcon(*icon);
	}

}

void TaskListWidget::showMsgMail(QModelIndex row1, QModelIndex row2)
{
	if (row1.model() == NULL)
		return;
	QString body = row1.model()->data(row1.sibling(row1.row(), this->tableMailView->model->fieldIndex("body"))).toString();
	this->setHtml(body);
}

void TaskListWidget::onclickButtonRefresh()
{
	//tableMailView->tableViewSql->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex,QModelIndex)));
	tableMailView->onClickToolButtonRefresh();
	/*
	QObject::connect(tableMailView->tableViewSql->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			 this, SLOT(showMsgMail(QModelIndex,QModelIndex)));
	QObject::connect(tableMailView->tableViewSql->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			 this, SLOT(setStatButton(QModelIndex,QModelIndex)));
			 */
}

void TaskListWidget::onclickButtonProp()
{
	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	int old_percentage = tableSql->model->record(row).value("percentage").toInt();
	if (row < 0)
		QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {
		DialogTaskStart* dialogTaskStart = new DialogTaskStart(this);
		dialogTaskStart->lineEditStatus->setReadOnly(true);
		dialogTaskStart->lineEditBegin->setReadOnly(true);
		dialogTaskStart->lineEditEnd->setReadOnly(true);
		dialogTaskStart->lineEditTaskBegin->setReadOnly(true);
		dialogTaskStart->lineEditTaskEnd->setReadOnly(true);
		dialogTaskStart->textEdit->setReadOnly(true);

		dialogTaskStart->spinBoxProcent->setValue(old_percentage);

		QDataWidgetMapper* mappermsg = new QDataWidgetMapper();

		mappermsg->setModel(tableSql->model);

		mappermsg->addMapping(dialogTaskStart->lineEditBegin, tableSql->model->fieldIndex("bdate"));
		mappermsg->addMapping(dialogTaskStart->lineEditEnd, tableSql->model->fieldIndex("edate"));
		mappermsg->addMapping(dialogTaskStart->lineEditTaskBegin, tableSql->model->fieldIndex("btaskdate"));
		mappermsg->addMapping(dialogTaskStart->lineEditTaskEnd, tableSql->model->fieldIndex("etaskdate"));

		mappermsg->addMapping(dialogTaskStart->lineEditStatus, tableSql->model->fieldIndex("status_id"));
		dialogTaskStart->textEdit->setHtml(tableSql->model->record(row).value("comm").toString());

		mappermsg->setCurrentIndex(row);

		QString msgid = tableSql->model->data(tableSql->model->index(row, tableSql->model->fieldIndex("msg_id"))).toString();
		QSqlQuery query;
		if (!query.exec(QString("select task_status('%1')").arg(msgid))) {
			QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при получении статуса задачи\n") + query.lastError().text());
			return;
		}
		query.next();
		int task_status = query.value(0).toInt();

		if (task_status != 3 && task_status != 4)
			dialogTaskStart->spinBoxProcent->setReadOnly(true);

		if (dialogTaskStart->exec() == QDialog::Accepted) {

			if (old_percentage != dialogTaskStart->spinBoxProcent->value()) {

				if (!query.exec(QString("UPDATE prof_msgtask SET percentage=%2 WHERE msg_id='%1'").arg(msgid).arg(dialogTaskStart->spinBoxProcent->value()))) {
					QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при изменении процента выполнения задачи\n") + query.lastError().text());
				} else
					tableSql->select();
			}
		}

		delete dialogTaskStart;
	}

}

void TaskListWidget::onclickButtonForward()
{
	TableSqlWidget* tableSql = qobject_cast<TableSqlWidget*>(sender()->parent());

	int row = tableSql->tableViewSql->currentIndex().row();
	QString recipient = tableSql->model->record(row).value("recipient_id").toString();

	if (row < 0)
		   QMessageBox::critical(0, "", tr("Не выбрана строка"));
	else {

		DialogForward* dialogForward = new DialogForward();
		if (dialogForward->exec() == QDialog::Accepted) {

			QSqlTableModel* pmodel = dialogForward->model;
			QString user_id = pmodel->data(pmodel->index(dialogForward->comboBoxUser->currentIndex(),pmodel->fieldIndex("user_id"))).toString();

			QSqlQuery query;
			if (!query.exec(QString("select msg_forward('%1','%2')").arg(recipient).arg(user_id))) {
				QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка при перепоручении\n") + query.lastError().text());
			}
		}
		delete dialogForward;
	}
	return;
}
