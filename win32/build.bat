mkdir ..\bin
copy %QTDIR%\bin\QtXmlPatterns4.dll ..\bin\
copy %QTDIR%\bin\QtMediaServices4.dll   ..\bin\
copy %QTDIR%\bin\QtOpenGL4.dll ..\bin\
copy %QTDIR%\bin\libgcc_s_dw2-1.dll ..\bin\
copy %QTDIR%\bin\assistant.exe ..\bin\
copy %QTDIR%\bin\QtHelp4.dll ..\bin\
copy %QTDIR%\bin\QtMultimedia4.dll ..\bin\
copy %QTDIR%\bin\QtSql4.dll ..\bin\
copy %QTDIR%\bin\QtCLucene4.dll ..\bin\
copy %QTDIR%\bin\QtGui4.dll ..\bin\
copy %QTDIR%\bin\QtWebKit4.dll ..\bin\
copy %QTDIR%\bin\QtCore4.dll ..\bin\
copy %QTDIR%\bin\QtNetwork4.dll ..\bin\
copy %QTDIR%\bin\QtXml4.dll ..\bin\
copy %QTDIR%\bin\mingwm10.dll ..\bin\
copy %QTDIR%\bin\phonon4.dll ..\bin\
copy drv\*.* ..\bin\
mkdir ..\bin\sqldrivers
copy sqldrivers\*.* ..\bin\sqldrivers
copy nsis\profimax_client.nsi ..\bin\


rem set PATH=%PATH%;D:\qt\qt\bin;D:\Qt\bin;D:\Qt\mingw\bin;%SystemRoot%\System32
cd ..
cd profimaxrelationalmodel
qmake
make -j 4 release
cd ..
cd profimaxdelegate
qmake
make -j 4 release
cd ..
cd qcboxbutton
qmake
make -j 4 release
cd ..
cd tablesqlwidget
qmake
make -j 4 release
cd ..
cd treesqlwidget
qmake
make -j 4 release
cd ..
cd profimaxreport
qmake
make -j 4 release
cd ..
cd profimax 
qmake
make -j 4 release
cd ..
cd plug_relcom 
qmake
make -j 4 release
cd ..
cd plug_workflow 
qmake
make -j 4 release
cd ..
cd plug_admin 
qmake
make -j 4 release
