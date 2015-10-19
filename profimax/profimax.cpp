/*
 *      main.cpp
 *
 *      Copyright 2009 Rodionov Andrey <andrey@roand.ru>
 *
 */
#include "mainwindow.h"
#include <QtGui>


int main(int argc, char* argv[]) {


	QApplication *app = new QApplication(argc,argv);
#if QT_VERSION < 0x050000
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
	MainWindow *window = new MainWindow();
	if (window->onActionConnect()==false)
		return 0;

	window->loadPlugins();
	window->readSettings();
	window->show();

	return app->exec();
}
