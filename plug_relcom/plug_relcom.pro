# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 7 12:01:48 2009
# #####################################################################
TEMPLATE = lib
DESTDIR = ../bin/plugins
CONFIG += plugin \

QT += core \
    gui \
    sql \
    svg \
    xml \
    network \
    uitools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_RPATHDIR += $$PWD/../bin 
LIBS +=-L$$PWD/../bin -ltablesqlwidget \
    -ltreesqlwidget \
    -lprofimaxreport \
    -lprofimaxrelationalmodel \
    -lqcboxbutton \
    -lprofimaxdelegate

DEPENDPATH += .
INCLUDEPATH += . \
    ../tablesqlwidget \
    ../treesqlwidget \
    ../profimaxreport \
    ../profimaxrelationalmodel \
    ../qcboxbutton \
    ../profimaxdelegate

# Input
HEADERS += dialogeditotdel.h \
           dialogsotr.h \
           dialogvibsotr.h \
           plug_relcom.h \
           qsqltreeviewmodel.h \
           tablesqldolgn.h \
           tablesqlobraz.h \
           treesqlotdel.h \
           ../profimax/interfaces.h \
    dialogkvp.h \
    dialogppo.h \
    dialogswitchppo.h
FORMS += dialogeditotdel.ui dialogsotr.ui dialogvibsotr.ui \
    dialogkvp.ui \
    dialogppo.ui \
    dialogswitchppo.ui
SOURCES += dialogeditotdel.cpp \
           dialogsotr.cpp \
           dialogvibsotr.cpp \
           plug_relcom.cpp \
           qsqltreeviewmodel.cpp \
           tablesqldolgn.cpp \
           tablesqlobraz.cpp \
           treesqlotdel.cpp \
           ../profimax/interfaces.cpp \
    dialogkvp.cpp \
    dialogppo.cpp \
    dialogswitchppo.cpp
RESOURCES += qrc.qrc

win32 {
   COPYREPORT = $$system(copyreport.bat)
}
unix {
   COPYREPORT = $$system(./copyreport.sh)
}

