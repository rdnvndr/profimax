/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "welcomewidget.h"

#include "spravtoolbutton.h"


WelcomeWidget::WelcomeWidget(QWidget* pwgt) : QWidget(pwgt) {
	setupUi(this);

	setAcceptDrops(true);
	flowLayout = new FlowLayout;

	this->horizontaMain->addLayout(flowLayout);

}

WelcomeToolButton* WelcomeWidget::addButton(QString text, QIcon icon){

	WelcomeToolButton *toolButton = new WelcomeToolButton();

	// Текст
	toolButton->setText(text);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
	toolButton->setSizePolicy(sizePolicy);
	// Иконка
	//QIcon icon(QString::fromUtf8(":/about/icons/import.png"));
	toolButton->setIcon(icon);
	toolButton->setIconSize(QSize(32, 32));
	toolButton->setMinimumSize(QSize(100, 0));
	toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolButton->setAutoRaise(true);
	//toolButton->setStyleSheet("background-color: rgb(139, 153, 222);");
	connect(toolButton,SIGNAL(removeButton()),this,SLOT(removeButton()));

	//QMessageBox::critical(this,tr("Ошибка"),QApplication::activeWindow()->objectName());

	flowLayout->addWidget(toolButton);


	return toolButton;
}

void WelcomeWidget::removeButton(){

	QWidget *widget = qobject_cast<QWidget *>(sender());
	//QMessageBox::critical(this,tr("Ошибка"),widget->objectName());

	QSettings* settings = new QSettings("Roand", "ProfiMAX");
	QStringList list;
	settings->beginGroup("WelcomeWidget");
	list = settings->value("WelcomeWidget",QStringList()).toStringList();
	for(int i=list.count()-1;i>=0;i--)
	    if (list.at(i)==widget->objectName())
			list.removeAt(i);
	settings->setValue("WelcomeWidget",list);
	settings->endGroup();
	delete settings;

	flowLayout->removeWidget(widget);
	widget->deleteLater();

}

 void WelcomeWidget::dropEvent(QDropEvent *event){
	MyTreeWidget* treeWidget = QApplication::activeWindow()->findChild<MyTreeWidget*>(event->mimeData()->text());

	if (treeWidget==NULL||treeWidget->iSprav==NULL)
    	    return;

	QIcon icon = qvariant_cast<QIcon>(event->mimeData()->imageData());
	QString objectName = treeWidget->objectName()+"."+treeWidget->iSprav->getWidgetName(treeWidget->currentItem());

	WelcomeToolButton* checkWelcome = this->findChild<WelcomeToolButton*>(objectName);
	if (treeWidget!=NULL)
	{
		if (checkWelcome!=NULL)
		    if (checkWelcome->text()==treeWidget->currentItem()->text(0))
			return;

		WelcomeToolButton* wbutton = addButton(treeWidget->currentItem()->text(0),icon);
		wbutton->setObjectName(objectName);

		wbutton->iSprav = treeWidget->iSprav;
		connect(wbutton, SIGNAL(clicked()),QApplication::activeWindow(), SLOT(callFuncAction()));

		QSettings* settings = new QSettings("Roand", "ProfiMAX");
		QStringList list;

		settings->beginGroup("WelcomeWidget");

		list = settings->value("WelcomeWidget",QStringList()).toStringList();

		list << objectName;
		settings->setValue("WelcomeWidget",list);

		settings->endGroup();
		delete settings;
	}
	event->acceptProposedAction();
 }

 void WelcomeWidget::dragEnterEvent(QDragEnterEvent *event)
 {
	 // if (event->mimeData()->hasFormat("text/plain"))
		 event->acceptProposedAction();
 }

 void WelcomeWidget::restoreButton(){

	 QSettings* settings = new QSettings("Roand", "ProfiMAX");
	 QStringList list;
	 settings->beginGroup("WelcomeWidget");
	 list = settings->value("WelcomeWidget",QStringList()).toStringList();
	 settings->endGroup();
	 delete settings;

	 foreach (QString text,list){
	    QStringList llist = text.split(".");
	    MyTreeWidget* treeWidget = this->parent()->findChild<MyTreeWidget*>(llist.at(0));
	    if (treeWidget!=NULL){
		QTreeWidgetItem* item = treeWidget->iSprav->getItem(llist.at(1));
		if (item!=NULL){

		    WelcomeToolButton* wbutton = addButton(item->text(0),item->icon(0));

		    wbutton->iSprav = treeWidget->iSprav;
		    wbutton->setObjectName(text);
		    connect(wbutton, SIGNAL(clicked()),this->parent(), SLOT(callFuncAction()));
		}
	    }
	}

 }

