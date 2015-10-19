#ifndef DIALOGTASKSTART_H
#define DIALOGTASKSTART_H
#include "ui_dialogtaskstart.h"
class DialogTaskStart: public QDialog, public Ui::DialogTaskStart{
    Q_OBJECT
    public:
	DialogTaskStart(QWidget* pwgt = 0);
};

#endif // DIALOGTASKSTART_H
