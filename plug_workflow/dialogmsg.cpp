/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */
#include <QMessageBox>
#include <QtPrintSupport/QPrintDialog>
#include <QSpinBox>
#include <QDateEdit>
#include "dialogmsg.h"

DialogMsg::DialogMsg(QWidget* pwgt) : QMainWindow(pwgt) {
	setupUi(this);

	removemsgid=tr("");
	// Создание столбцов и строк
	amodel= new QStandardItemModel(4, 2);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 2; ++column) {
			QStandardItem *item = new QStandardItem(QString(""));
			amodel->setItem(row, column, item);
		}
	}
	tableViewAttr->setModel(amodel);
	amodel->setHeaderData(0, Qt::Horizontal, tr("Наименование"));
	amodel->setHeaderData(1, Qt::Horizontal, tr("Значение параметра"));
	tableViewAttr->setColumnWidth(0,120);




	// обработчики событий
	connect(actionSend, SIGNAL(triggered()), this, SLOT(onActionSend()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(onActionSave()));
	connect(actionPrint, SIGNAL(triggered()), this, SLOT(onActionPrint()));
	connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(onActionPrintPreview()));
	connect(actionExportPDF, SIGNAL(triggered()), this, SLOT(onActionExportPDF()));

	connect(actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
	connect(actionCut, SIGNAL(triggered()), this, SLOT(onActionCut()));
	connect(actionPaste, SIGNAL(triggered()), this, SLOT(onActionPaste()));

	connect(actionUndo, SIGNAL(triggered()), this, SLOT(onActionUndo()));
	connect(actionRedo, SIGNAL(triggered()), this, SLOT(onActionRedo()));
	connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(onActionSelectAll()));

	connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*,QWidget*)),
			 this, SLOT(onFocusChanged(QWidget*,QWidget*)));


}

void DialogMsg::setModel(ProfimaxRelationalModel *model){
	this->model = model;
	ComboBoxMailDelegate* combodeleg = new ComboBoxMailDelegate();
	combodeleg->setModel(model);
	QList<int> headName;
	headName << model->fieldIndex("sender")  << model->fieldIndex("recipient")
		 << model->fieldIndex("type_id")<<model->fieldIndex("prior_id")
		 << model->fieldIndex("bdate")<<model->fieldIndex("edate");
	combodeleg->setFieldName(headName);
	tableViewAttr->setItemDelegate(combodeleg);
}

// Создать сообщение
void DialogMsg::onActionSend(){

    QStringList field;
    field <<"stat_id";
    QStringList val;
    val <<"1";
    insertMessage(field,val);
}

void DialogMsg::insertMessage(QStringList field, QStringList val){
    QString fieldsql = "";
    QString valsql = "";
    QStringList reciplist;
    QString upval = "";

    for (int i=0;i<field.count();i++) {
	fieldsql+= field.at(i);
	valsql+=val.at(i);
	upval += QString("%1 = %2").arg(field.at(i)).arg(val.at(i));
    }

    if (fieldsql!=tr("")){
	upval = QString("%1 = %2, ").arg(fieldsql).arg(valsql);
	fieldsql+= tr( ", ");
	valsql += tr( ", ");
    }

    for (int i=0;i<amodel->rowCount();i++) {

	QString fieldname;
	if (amodel->item(i,0)!=NULL)
	    fieldname = amodel->item(i,0)->text();

	if (fieldname!=QString("")) {
	    QString valname = amodel->item(i,1)->text().replace("'","\"");
	    if (fieldname==tr("Кому")){
		reciplist << valname;
	    }else if (fieldname==tr("Тип")){
		fieldsql+="type_id, ";
		valsql+=QString("%1, ").arg(valname);
		upval += QString("type_id = %1, ").arg(valname);
	    }else if (fieldname==tr("Приоритет")){
		fieldsql+="prior_id, ";
		valsql+=QString("%1, ").arg(valname);
		upval += QString("prior_id = %1, ").arg(valname);
	    }else if (fieldname==tr("Начало")){
		fieldsql+="bdate, ";
		valsql+=QString("'%1', ").arg(valname);
		upval += QString("bdate = '%1', ").arg(valname);
	    }else if (fieldname==tr("Конец")){
		fieldsql+="edate, ";
		valsql+=QString("'%1', ").arg(valname);
		upval += QString("edate = '%1', ").arg(valname);
	    }
	}
    }

    // Вставка значений
    fieldsql += QString("subj, ");
    valsql += QString("'%1', ").arg(lineEditSubj->text().replace("'","\""));
    upval += QString("subj = '%1', ").arg(lineEditSubj->text().replace("'","\""));

    fieldsql += QString("body ");
    valsql += QString("'%1' ").arg(textEditMsg->textEdit->toHtml().replace("'","\""));
    upval += QString("body = '%1' ").arg(lineEditSubj->text().replace("'","\""));

    QSqlQuery query;

    // Если черновик на редактирование то обновляем
    if (model->tableName()==tr("prof_msg_draft") && removemsgid!=tr("")){

	QSqlDatabase::database().transaction();
	if (!query.exec(QString("UPDATE  %1 SET %2 WHERE msg_id = '%3'")
	    .arg(QString(model->tableName())).arg(upval).arg(removemsgid))){
		QSqlDatabase::database().rollback();
		QMessageBox::critical(this,tr("Ошибка"),tr("Ошибка обновления сообщения\n")+query.lastError().text());
	} else
	    if (!query.exec(QString("DELETE FROM %1 WHERE msg_id = '%2'")
		.arg(QString("prof_msgrecipient"))
		.arg(removemsgid))){
		    QSqlDatabase::database().rollback();
		    QMessageBox::critical(this,tr("Ошибка"),tr("Ошибка при удалении получателя сообщения\n")+query.lastError().text());
	    } else
		for (int i=0;i<reciplist.count();i++)
		    if (!query.exec(QString("INSERT INTO %1 (user_id, msg_id) values ('%2','%3')")
		    .arg(QString("prof_msgrecipient"))
		    .arg(reciplist.at(i))
		    .arg(removemsgid))){
			QSqlDatabase::database().rollback();
			QMessageBox::critical(this,tr("Ошибка"),tr("Ошибка при создании получателя сообщения\n")+query.lastError().text());
		    }
	QSqlDatabase::database().commit();

    } else {
	// Получение UUID
	QSqlDatabase::database().transaction();
	query.exec("select uuid_generate()");
	query.next();
	QString curidx = query.value(0).toString();

	// Должно быть в одной транзакции
	if (!query.exec(QString("INSERT INTO %1 ( msg_id, %2 ) VALUES ( '%4', %3 )")
	    .arg(QString("prof_msg_inbox")).arg(fieldsql).arg(valsql).arg(curidx))){
		QSqlDatabase::database().rollback();
		QMessageBox::critical(this,tr("Ошибка"),tr("Ошибка при создании сообщения\n")+query.lastError().text());
	} else
	    for (int i=0;i<reciplist.count();i++)
		if (!query.exec(QString("INSERT INTO %1 (user_id, msg_id) values ('%2','%3')")
		    .arg(QString("prof_msgrecipient"))
		    .arg(reciplist.at(i))
		    .arg(curidx))){
			QSqlDatabase::database().rollback();
			QMessageBox::critical(this,tr("Ошибка"),tr("Ошибка при создании получателя сообщения\n")+query.lastError().text());
		}
	QSqlDatabase::database().commit();
    }

    this->close();
}

// Создать черновик
void DialogMsg::onActionSave(){
    QStringList field;
    field <<"stat_id";
    QStringList val;
    val <<"2";
    insertMessage(field,val);
}

void DialogMsg::closeEvent(QCloseEvent *event){
	event->accept();
 }

void DialogMsg::onActionPrint()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *dlg = new QPrintDialog(&printer, this);

	dlg->setWindowTitle(tr("Печать сообщения"));
	if (dlg->exec() == QDialog::Accepted) {
		textEditMsg->print(&printer);
	}
	delete dlg;
#endif
}

void DialogMsg::onActionPrintPreview(){
	textEditMsg->previewPrint();
}

void DialogMsg::onActionExportPDF(){
	textEditMsg->printPdf();
}

void DialogMsg::onActionCut() {
	 if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)

	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_X, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onActionCopy() {
	if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)
	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_C, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onActionPaste() {
	if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)
	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_V, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onActionUndo() {
	if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)
	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_Z, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onActionRedo() {
	if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)
	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_Y, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onActionSelectAll() {
	if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)
	){
		QKeyEvent event(QEvent::KeyPress,Qt::Key_A, Qt::ControlModifier);
		QApplication::sendEvent(QApplication::focusWidget() , &event);
	}
}

void DialogMsg::onFocusChanged(QWidget* old,QWidget* now){
		if ((qobject_cast<QLineEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QSpinBox *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QDateEdit *>(QApplication::focusWidget())!=NULL)||
		 (qobject_cast<QTextEdit *>(QApplication::focusWidget())!=NULL)){

			actionCut->setEnabled(true);
			actionCopy->setEnabled(true);
			actionPaste->setEnabled(true);
			actionRedo->setEnabled(true);
			actionUndo->setEnabled(true);
			actionSelectAll->setEnabled(true);

		}else{
			actionCut->setDisabled(true);
			actionCopy->setDisabled(true);
			actionPaste->setDisabled(true);
			actionRedo->setDisabled(true);
			actionUndo->setDisabled(true);
			actionSelectAll->setDisabled(true);
		}
}

void DialogMsg::addRowAttr(QString name, QString value,int row){
    if (row>(amodel->rowCount()-2))
	amodel->insertRow(row);
    amodel->setItem(row, 0, new QStandardItem(name));
    amodel->setItem(row, 1, new QStandardItem(value));
}

void DialogMsg::loadMessage(QString msgid){
    int row=0;

    QSqlQuery query;

    query.exec(QString("SELECT * FROM %1 WHERE msg_id = '%2'").arg(tr("prof_msgrecipient")).arg(msgid));
    while (query.next()){
	addRowAttr(tr("Кому"),query.value(query.record().indexOf("user_id")).toString(),row);
	row++;
    }


    query.exec(QString("SELECT * FROM %1 WHERE msg_id = '%2'").arg(model->tableName()).arg(msgid));
    query.next();

    addRowAttr(tr("Приоритет"),query.value(query.record().indexOf("prior_id")).toString(),row);
    row++;

    addRowAttr(tr("Тип"),query.value(query.record().indexOf("type_id")).toString(),row);
    row++;

    addRowAttr(tr("Начало"),query.value(query.record().indexOf("bdate")).toString(),row);
    row++;

    addRowAttr(tr("Конец"),query.value(query.record().indexOf("edate")).toString(),row);
    row++;

    lineEditSubj->setText(query.value(query.record().indexOf("subj")).toString());
    textEditMsg->setHtml(query.value(query.record().indexOf("body")).toString());
    removemsgid = msgid;


}
