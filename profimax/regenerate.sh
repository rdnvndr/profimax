#!/bin/sh

TVER=`cat version.h | grep '#define PROFIMAX_REV'`
VER=`git describe --tags --always`

if [ "$TVER" != "#define PROFIMAX_REV \"$VER\"" ]; then
  echo "#ifndef VERSION_H" > version.h
  echo "#define VERSION_H" >> version.h
  echo "#define PROFIMAX_REV \"$VER\"" >> version.h
  echo "#endif" >> version.h
fi


