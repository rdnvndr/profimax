#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H
#include "tablesqlwidget.h"
#include "texteditwidget.h"
#include "ui_tasklistwidget.h"
#include "dialogmsg.h"
#include <QtGui>

class TaskListWidget : public QWidget, public Ui::TaskListWidget{
        Q_OBJECT
    private:
	bool flagbutton;
    public:
	QFrame* lineToolView;
	QToolButton* toolButtonStart;
	QToolButton* toolButtonFinish;
	QToolButton* toolButtonProp;
	QToolButton* toolButtonForward;

        TaskListWidget(QWidget* pwgt = 0);
        void setHtml(const QString &text);
        void createTaskButton();

	void setTable(const QString & tableName);
	void setHeader(const QString & col, const QString & colname);
	void setRelation(QString mycol,QString table,QString linkcol,QString showcol);
	void setSortColumn(QStringList stl);
	void setFieldNameBool(QStringList Name);
	void select();

    private slots:
	    void editDialogMsg();
	    void addDialogMsg();
	    void deleteDialogMsg();
	    void showMsgMail(QModelIndex row1,QModelIndex row2);
            void startTask();
            void stopTask();
            void finishTask();
	    void refuseTask();
	    void setStatButton(QModelIndex row1,QModelIndex row2);
	    void onclickButtonRefresh();
	    void onclickButtonProp();
            void onclickButtonForward();
};

#endif // TASKLISTWIDGET_H
