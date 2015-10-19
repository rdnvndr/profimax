#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include <QMenu>
#include <interfaces.h>

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QMenu menu;

    explicit MyTreeWidget(QWidget *parent = 0);
    SpravInterface* iSprav;
    QPoint m_ptDragPos;


    virtual void mousePressEvent(QMouseEvent* pe);
    virtual void mouseMoveEvent(QMouseEvent* pe);
    void  startDrag();

    QString	     getPathFromItem(QTreeWidgetItem* item);
    QTreeWidgetItem* getItemFromPath(QString list);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    bool checkFormat(QDropEvent *ev);
    void dragMoveEvent(QDragMoveEvent *ev);

public slots:
    void showAllItem();
    bool searchShowItem(QString text,QTreeWidgetItem* item);
    void searchShowAllItem(QString text);

signals:
    void contextMenuSignal(QContextMenuEvent *event);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MYTREEWIDGET_H
