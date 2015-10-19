@echo off

for /f "tokens=*" %%i in ('git describe --tags --always') do (
   set ver=%%i
)
set test=-
if exist version.h (
  for /F "TOKENS=2,3*" %%i in (version.h) do ( 
    set test=%%j
  ) 
)

if not %test%=="%ver%" (
  echo #ifndef VERSION_H > version.h
  echo #define VERSION_H >> version.h
  echo #define PROFIMAX_REV "%ver%" >> version.h
  echo #endif >> version.h
)