#ifndef TABWIDGETMDI_H
#define TABWIDGETMDI_H

#include <QTabWidget>
#include  <QtGui>

class TabWidgetMDI : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetMDI(QWidget *parent = 0);
    QPoint m_ptDragPos;

    virtual void mousePressEvent(QMouseEvent* pe);
    virtual void mouseMoveEvent(QMouseEvent* pe);
    void  startDrag();

signals:

public slots:

};

#endif // TABWIDGETMDI_H
