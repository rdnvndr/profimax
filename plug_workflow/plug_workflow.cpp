#include "plug_workflow.h"

#include <QMessageBox>

#include <math.h>
#include <stdlib.h>
#include "tasklistwidget.h"
#include "dialogmsg.h"
#include "dialogmsgfolder.h"


WorkflowPlugin::WorkflowPlugin()
{
	ModulName = tr("Документооборот");
	ModulIcon = QIcon(tr(":icon/icons/inbox.png"));
}

void  WorkflowPlugin::callTabAction(QWidget *widget,QTreeWidgetItem* item){
    TaskListWidget* taskList = qobject_cast<TaskListWidget *>(widget);
    if (filter.value(item)==taskList->tableMailView->model->filter())
	return;

    QString old_fullfilter = taskList->tableMailView->model->filter();
    QString old_filter = taskList->tableMailView->filter;
    taskList->tableMailView->setFilter(filter.value(item));

    if (old_filter!=old_fullfilter)
	taskList->tableMailView->onClickToolButtonFilterApply();

    taskList->select();
    return;
}

QWidget* WorkflowPlugin::getSubWindowAll(QString sprav) {

    /*QStringList splitsprav = sprav.split("-");
    QString fullsprav = sprav;
    sprav = splitsprav.at(0);*/

    //QMessageBox::critical(NULL,tr("Ошибка"),filter.value(fullsprav));
    if (sprav==tr("Inbox")||sprav==tr("Outbox")||sprav==tr("Draft")||sprav==tr("Delete")){

		TaskListWidget* taskList = new TaskListWidget();

		if (sprav==tr("Inbox"))
		    taskList->setTable(tr("prof_msg_inbox"));
		else if (sprav==tr("Outbox"))
		    taskList->setTable(tr("prof_msg_outbox"));
		else if (sprav==tr("Draft"))
		    taskList->setTable(tr("prof_msg_draft"));
		else
		    taskList->setTable(tr("prof_msg_del"));

		taskList->setRelation("status_id","prof_msgstatus", "status_id", "status");
		taskList->setRelation("prior_id","prof_msgprior", "prior_id", "prior");
		taskList->setRelation("type_id","prof_msgtype", "type_id", "type");
		taskList->setRelation("sender","prof_user_view", "user_id", "lastname");
		taskList->setRelation("recipient","prof_user_view", "user_id", "lastname");
                taskList->setRelation("forwarder","prof_user_view", "user_id", "lastname");
		taskList->setRelation("executor","prof_user_view", "user_id", "lastname");

		QStringList FieldNameBool;
		FieldNameBool <<"ready";
		taskList->setFieldNameBool(FieldNameBool);

		taskList->select();

		QStringList headName;
		headName << "subj"<< "ready" << "sender" << "recipient"<<"type_id"
			 <<"prior_id"<<"bdate"<<"edate"<<"status_id"
                         <<"btaskdate"<<"etaskdate"<<"percentage"<<"executor"<<"forwarder"<<"created";
		taskList->setSortColumn(headName);

		taskList->setHeader("subj", tr("Тема"));
		taskList->setHeader("sender", tr("От кого"));
		taskList->setHeader("ready", tr("Прочитано"));
		taskList->setHeader("bdate", tr("Начало"));
		taskList->setHeader("edate", tr("Конец"));
		taskList->setHeader("recipient", tr("Кому"));
		taskList->setHeader("type_id", tr("Тип"));
		taskList->setHeader("status_id", tr("Статус"));
		taskList->setHeader("prior_id", tr("Приоритет"));
		taskList->setHeader("btaskdate", tr("Начато"));
		taskList->setHeader("etaskdate", tr("Закончено"));
		taskList->setHeader("percentage", tr("Выполнено"));
		taskList->setHeader("executor", tr("Исполнитель"));
                taskList->setHeader("forwarder", tr("Поручитель"));
                taskList->setHeader("created", tr("Создано"));

		if (sprav==tr("Inbox"))
		    taskList->createTaskButton();

		return taskList;
	}
	return NULL;
}

QList<QTreeWidgetItem *> WorkflowPlugin::getSpravItems(){
     QList<QTreeWidgetItem *> items;

     QTreeWidgetItem *item = new QTreeWidgetItem();
     QFont font = item->font(0);
     font.setBold(true);
     item->setFont(0,font);
     item->setText(0,tr("Сообщения"));
     items.append(item);

     QTreeWidgetItem *item1 = new QTreeWidgetItem(item);
     item1->setIcon(0,QIcon(tr(":icon/icons/inbox.png")));
     item1->setText(0,tr("Входящие"));
     setWidgetName(item1,tr("Inbox"));

     item1 = new QTreeWidgetItem(item);
     item1->setIcon(0,QIcon(tr(":icon/icons/draft.png")));
     item1->setText(0,tr("Черновики"));
     setWidgetName(item1,tr("Draft"));

     item1 = new QTreeWidgetItem(item);
     item1->setIcon(0,QIcon(tr(":icon/icons/send.png")));
     item1->setText(0,tr("Отправленные"));
     setWidgetName(item1,tr("Outbox"));

     item1 = new QTreeWidgetItem(item);
     item1->setIcon(0,QIcon(tr(":icon/icons/trash.png")));
     item1->setText(0,tr("Удаленные"));
     setWidgetName(item1,tr("Delete"));

     item = new QTreeWidgetItem();
     item->setFont(0,font);
     item->setText(0,tr("Отчеты"));
     items.append(item);

     // Считывание фильтров
     QSettings* settings = new QSettings("Roand", "ProfiMAX");
     QStringList list;
     settings->beginGroup("plug_workflow");
     list = settings->value("msg_filter",QStringList()).toStringList();


     for(int i=0;i<list.count();i=i+3){
	 item = this->getItem(list.at(i).split("-").at(0));
	 item1 = new QTreeWidgetItem(item);
	 item1->setIcon(0,QIcon(tr(":icon/icons/bookmarks.png")));
	 item1->setText(0,list.at(i+1));
	 setWidgetName(item1,list.at(i).mid(ModulName.length()));
	 filter.insert(item1,list.at(i+2));
     }

     settings->endGroup();
     delete settings;

     return items;
}

bool WorkflowPlugin::dragTreeWidget(QDropEvent *event, QTreeWidget* tree){
    //QMessageBox::critical(NULL,tr("Ошибка"), tree->itemAt(event->pos())->text(0));

    if (getWidgetName(tree->itemAt(event->pos()))==event->mimeData()->text()){

	TaskListWidget* taskList = QApplication::activeWindow()->findChild<TaskListWidget*>(event->mimeData()->text());
	if (taskList!=NULL){

	    if (taskList->tableMailView->model->filter()==""){
		QMessageBox::critical(NULL,tr("Ошибка"), tr("Не могу создать элемент, задайте пожалуйста фильтр."));
		return false;
	    }
	    DialogMsgFolder* dialogMsgFolder = new DialogMsgFolder();
	    if (dialogMsgFolder->exec()!=QDialog::Accepted)
		return false;
	    QString itemName = dialogMsgFolder->lineEditName->text();

	    QTreeWidgetItem *item1 = new QTreeWidgetItem(tree->itemAt(event->pos()));
	    item1->setIcon(0,QIcon(tr(":icon/icons/bookmarks.png")));
	    item1->setText(0,itemName);

	    filter.insert(item1,taskList->tableMailView->model->filter());

	    setWidgetName(item1,event->mimeData()->text().mid(ModulName.length())+"-"+itemName);

	    QSettings* settings = new QSettings("Roand", "ProfiMAX");
	    QStringList list;
	    settings->beginGroup("plug_workflow");
	    list = settings->value("msg_filter",QStringList()).toStringList();

	    list << getWidgetName(tree->itemAt(event->pos()))+"-"+itemName <<itemName <<taskList->tableMailView->model->filter();
	    settings->setValue("msg_filter",list);

	    settings->endGroup();
	    delete settings;


	    return true;
	}
    }

    return false;
}

void WorkflowPlugin::contextMenu(QContextMenuEvent *event){
    QTreeWidget* tree = qobject_cast<QTreeWidget *>(sender());
    QTreeWidgetItem *item = tree->itemAt(event->pos());
     if (filter.value(item)=="")
	 return;

    // добавление меню
    QMenu* menu = new QMenu();

    QAction* action = new QAction(tr("Переименовать"),tree);
    menu->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(renameFilter()));

    action = new QAction(tr("Удалить"),tree);
    menu->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(removeFilter()));

    menu->exec(event->globalPos());
    return;
}

void WorkflowPlugin::renameFilter(){
    QTreeWidget* tree = qobject_cast<QTreeWidget *>(sender()->parent());
    QTreeWidgetItem *item = tree->currentItem();

    DialogMsgFolder* dialogMsgFolder = new DialogMsgFolder();
    if (dialogMsgFolder->exec()!=QDialog::Accepted)
	return;
    QString itemName = dialogMsgFolder->lineEditName->text();


    QSettings* settings = new QSettings("Roand", "ProfiMAX");
    QStringList list;
    settings->beginGroup("plug_workflow");
    list = settings->value("msg_filter",QStringList()).toStringList();

    for (int i=list.count()-1;i>=0;i=i-3){

	if (list.at(i-2)==getWidgetName(item)) {
	    list.replace(i-1,itemName);
	}
    }
    settings->setValue("msg_filter",list);
    settings->endGroup();
    delete settings;

    item->setText(0,itemName);

    return;
}

void WorkflowPlugin::removeFilter(){
    QTreeWidget* tree = qobject_cast<QTreeWidget *>(sender()->parent());
    QTreeWidgetItem *item = tree->currentItem();

    QSettings* settings = new QSettings("Roand", "ProfiMAX");
    QStringList list;
    settings->beginGroup("plug_workflow");
    list = settings->value("msg_filter",QStringList()).toStringList();

    for (int i=list.count()-1;i>=0;i=i-3){

	if (list.at(i-2)==getWidgetName(item)) {
	    list.removeAt(i);
	    list.removeAt(i-1);
	    list.removeAt(i-2);
	}
    }
    settings->setValue("msg_filter",list);
    settings->endGroup();
    delete settings;

    item->parent()->removeChild(item);


    return;
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(plug_workflow, WorkflowPlugin)
#endif

