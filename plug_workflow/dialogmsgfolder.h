#ifndef DIALOGMSGFOLDER_H
#define DIALOGMSGFOLDER_H
#include <QtGui>
#include "ui_dialogmsgfolder.h"

class DialogMsgFolder:public QDialog, public Ui::DialogMsgFolder{
	Q_OBJECT
public:
    DialogMsgFolder();
};



#endif // DIALOGMSGFOLDER_H
