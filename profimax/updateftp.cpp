#include "updateftp.h"
#include <QMessageBox>

UpdateFTP::UpdateFTP(QWidget *parent)
	:QWidget(parent),ftp(0)
{
	abort = false;
}

void UpdateFTP::ftpConnect(const QUrl &url, bool active)
{

#ifndef QT_NO_CURSOR
	setCursor(Qt::WaitCursor);
#endif

    ftp = new QFtp(this);

	if (active)
        ftp->setTransferMode(QFtp::Active);
	else
        ftp->setTransferMode(QFtp::Passive);

	connect(ftp, SIGNAL(done(bool)),
			this, SLOT(ftpDone(bool)));
	connect(ftp, SIGNAL(rawCommandReply( int, const QString& )),
			this, SLOT(ftpReply( int, const QString& )));
	connect(ftp, SIGNAL(commandFinished(int, bool)),
			this, SLOT(ftpFinish(int, bool)));

	ftp->connectToHost(url.host(), 21);

	if (!url.userName().isEmpty())
		ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
	else
		ftp->login();
	upath="";
	 if (!url.path().isEmpty()){
		upath=url.path();
	 }
}

void UpdateFTP::ftpSize(QString filename){
	ftp->rawCommand(QString("size %1").arg(upath+filename));
}

void UpdateFTP::ftpReply(int replyCode, const QString & detail  ){
	qint64 size = detail .toInt();
	if (replyCode==213){
		typecommand=1;
		QString fileNameNoRoot = dfile.at(dfile.count()-1);
		fileNameNoRoot.remove(0,1);
		QFileInfo fileInfo(fileNameNoRoot);

		QFileInfo dFileInfo(fileInfo.absolutePath()+QString("/update_")+fileInfo.fileName());

		if (fileInfo.size()!=size&&size!=0&&!dFileInfo.exists()&&dFileInfo.size()!=size)
		{
			QFile* file = new QFile(fileInfo.absolutePath()+QString("/update_")+fileInfo.fileName());
			if (!file->open(QIODevice::WriteOnly)) {
				QMessageBox::information(this, tr("FTP Обновление"),
										 tr("Не могу сохранить фаил %1: %2.")
										 .arg(fileNameNoRoot).arg(file->errorString()));
			}else{
				int id = ftp->get(upath+dfile.at(dfile.count()-1), file);
				openedFiles[id] = file;
			}
		}
	}

	dfile.removeAt(dfile.count()-1);
	if (dfile.count()>0)
		ftpSize(dfile.at(dfile.count()-1));
}

void UpdateFTP::start()
{
	if (dfile.count()>0){
		ftpSize(dfile.at(dfile.count()-1));
		msgBox = new DialogUpdate(qobject_cast<QWidget *>(this->parent()));
		/*
		msgBox = new QMessageBox(qobject_cast<QWidget *>(this->parent()));
		msgBox->setWindowTitle(tr("Обновление программы"));
		msgBox->setText(tr("Обновление программы по сети."));
		msgBox->setInformativeText(tr("Внимание. Идет идет обновление программы по сети. Для отмены нажмите <Cancel>."));
		QPixmap icon(QLatin1String(":/about/icons/knetconfig.png"));
		msgBox->setIconPixmap(icon);
		msgBox->setStandardButtons(QMessageBox::Cancel);
		msgBox->setDefaultButton(QMessageBox::Cancel);*/
		if (msgBox->exec()==QDialog::Rejected){
			abort=true;
			ftpAbort();
		}
	}

}

void UpdateFTP::ftpAbort(){
	ftp->abort();
}

void UpdateFTP::ftpFinish(int id, bool error){
#ifndef QT_NO_CURSOR
	setCursor(Qt::ArrowCursor);
#endif

	if (ftp->error()&&ftp->errorString()!=tr("No such file or directory")){
			QMessageBox::critical(this, tr("Ошибка обновления"),ftp->errorString());
			abort=true;
	}

	if (abort)
		return;


    if (ftp->currentCommand() == QFtp::RawCommand&&error){
		if (dfile.count()>0)
				ftpSize(dfile.at(dfile.count()-1));
	}

    if (ftp->currentCommand() == QFtp::Get){
		if (error) {
			openedFiles[id]->close();
			openedFiles[id]->remove();
		}else {
			openedFiles[id]->close();
		}
	}
}

void UpdateFTP::ftpDone(bool error){

	msgBox->close();
	delete msgBox;
	if (abort)
		return;

	ftpRenameFile(QString(""));

	QMessageBox::information(this, tr("Обновление программы"),tr("Обновление завершено.\nПожалуйста  перезапустите программу."));

	QSettings* settings = new QSettings("Roand", "ProfiMAX");
	settings->beginGroup("Update");
	settings->setValue("update", true);
	settings->endGroup();

}

void UpdateFTP::ftpRenameFile(QString direct){

	QDir dir(direct);

	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); i++) {
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.fileName()!=QString(".")&&fileInfo.fileName()!=QString("..")){
				if (fileInfo.isDir()){
					ftpRenameFile(dir.absolutePath()+QString("/")+fileInfo.fileName());
				}else{

					if (fileInfo.fileName().mid(0,7)==QString("update_")){

						QFile fileNameOld(dir.absolutePath()+QString("/")+fileInfo.fileName().mid(7));
						fileNameOld.rename(dir.absolutePath()+QString("/")+"old_"+fileInfo.fileName().mid(7));

						QFile fileName(dir.absolutePath()+QString("/")+"update_"+fileInfo.fileName().mid(7));
						fileName.rename(dir.absolutePath()+QString("/")+fileInfo.fileName().mid(7));
					}
				}
			}
	 }
}

