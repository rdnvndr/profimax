######################################################################
# Automatically generated by qmake (2.01a) ?? ????? 25 23:09:57 2011
######################################################################

TEMPLATE = lib
DESTDIR = ../bin
QT      +=   core \
  gui \
  sql \
  svg \
  xml \
  network \
  script

DEPENDPATH += .
QMAKE_RPATHDIR += $$PWD/../bin 
LIBS +=-L$$PWD/../bin -lprofimaxrelationalmodel
INCLUDEPATH += . \
    ../profimaxrelationalmodel

# Input
HEADERS += profimaxdelegate.h
SOURCES += profimaxdelegate.cpp
