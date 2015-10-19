/*
 *      mainwindow.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 *
 */


#include "profimaxreport.h"
#include <QWebFrame>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QFileDialog>

ProfimaxReport::ProfimaxReport(QWidget* pwgt) : QWidget(pwgt) {
	setupUi(this);
}

// старая функцмя и подлежит удалению
QString ProfimaxReport::Replace(QString str){
	QString ret("");
	int p = 0;
	while (true==true){
		int b = str.indexOf("<!-- beg:",p);
		if (b==-1)
			break;

		int e = str.indexOf("-->",b);
		if (e==-1)
			break;

		QString param = str.mid(b+9,e-b-9).trimmed();

		int be = str.indexOf("<!-- end:",e+3);
		if (be==-1)
			break;

		int ee = str.indexOf("-->",be);
		if (ee==-1)
			break;

		//QMessageBox::information(this,tr("Информация"),param);
		QString s("");
		for (int i=0;i<insparam.count();i=i+2){
			if (insparam.at(i)==param){
				s=insparam.at(i+1);
				break;
			}
		}
		ret=ret+str.mid(p,e-p+3)+s+str.mid(be,ee-be+3);


		p=ee+3;


	}
	ret=ret+str.mid(p);
	return ret;
}

bool ProfimaxReport::load(const QString &f){

	if (!QFile::exists(QString("%1/%2").arg(QApplication::applicationDirPath()).arg(f)))
		return false;
	QFile file(QString("%1/%2").arg(QApplication::applicationDirPath()).arg(f));
	if (!file.open(QFile::ReadOnly))
		return false;

	QByteArray data = file.readAll();
	QTextCodec *codec = Qt::codecForHtml(data);
	QString str = codec->toUnicode(data);

	//str=Replace(str);

	/* Старая функция замена параметров
	for (int i=0;i<insparam.count();i=i+2){
		QString beghtml = "<!-- beg:"+insparam.at(i)+" -->";
		QString endhtml = "<!-- end:"+insparam.at(i)+" -->";
		str.replace(QRegExp(beghtml+".*"+endhtml),beghtml+insparam.at(i+1)+endhtml);
	}*/

	if (Qt::mightBeRichText(str)) {
		textEditReport->setHtml(str);
		for (int i=0;i<insparam.count();i=i+2){
			textEditReport->page()->mainFrame()->evaluateJavaScript(QString("setValue(\"%1\",\"%2\")").arg(insparam.at(i)).arg(insparam.at(i+1)));
		}
	} else {
		str = QString::fromLocal8Bit(data);
	}

	fileform =f;

	return true;
}

void ProfimaxReport::setReportName(QString reportname){
	this->reportname=reportname;
}

QString ProfimaxReport::getReportName(){
	return this->reportname;
}

QStringList ProfimaxReport::getInsertParam(){

	QStringList str;
	for (int i=0;i<insparam.count();i=i+2){
		QString parval= textEditReport->page()->mainFrame()->evaluateJavaScript(QString("getValue(\"%1\")").arg(insparam.at(i))).toString();
		str << insparam.at(i)<<parval;
	}
	return str;
}

void ProfimaxReport::filePrint()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *dlg = new QPrintDialog(&printer, this);
	/*
	if (textEditReport->textCursor().hasSelection())
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);*/
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted) {
		textEditReport->print(&printer);
	}
	delete dlg;
#endif
}

void ProfimaxReport::filePrintPreview()
{
#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer, this);

	if (!printer.isValid())
	  printer.setOutputFormat(QPrinter::PdfFormat);

	connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
	preview.exec();
#endif
}

void ProfimaxReport::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	textEditReport->print(printer);
#endif
}

void ProfimaxReport::filePrintPdf()
{
#ifndef QT_NO_PRINTER
	QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
													QString(), "*.pdf");
	if (!fileName.isEmpty()) {
		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(".pdf");
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		textEditReport->print(&printer);
}
#endif
}


void ProfimaxReport::setInsertParam(QStringList Name){
	insparam = Name;
}
