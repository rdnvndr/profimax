#ifndef UPDATEFTP_H
#define UPDATEFTP_H

#include <QDialog>
#include <QtGui>
#include <QtNetwork>
#include <QProgressDialog>
#include <QFtp>
#include "dialogupdate.h"

class UpdateFTP: public QWidget
{
	Q_OBJECT

public:


	QStringList dfile;
	int typecommand;
	int size;
	QHash<int,QFile*> openedFiles;
    QFtp *ftp;
	UpdateFTP(QWidget *parent = 0);
	QString upath;
	DialogUpdate* msgBox;
	bool abort;

public slots:
	void ftpConnect(const QUrl &url, bool active);

	void ftpAbort();

	void ftpDone(bool error);
	void start();
	void ftpSize(QString filename);
	void ftpFinish(int id, bool error);
	void ftpRenameFile(QString direct);

	void ftpReply( int replyCode, const QString & detail  );

private:
	QProgressDialog *progressDialog;



};

#endif
