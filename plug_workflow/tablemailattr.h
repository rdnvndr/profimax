#ifndef TABLEMAILATTR_H
#define TABLEMAILATTR_H

#include <QTableView>
#include <QtGui>
#include <QtCore>

class TableMailAttr : public QTableView
{
    Q_OBJECT
public:
    explicit TableMailAttr(QWidget *parent = 0);
    void keyPressEvent ( QKeyEvent * event );

signals:

public slots:

};

#endif // TABLEMAILATTR_H
