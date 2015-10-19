; profimax.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install profimax.nsi into a directory that the user selects,

;--------------------------------

!include nsDialogs.nsh
!include LogicLib.nsh

; The name of the installer
Name "ProfiMAX Server"

; The file to write
OutFile "..\profimax_server.exe"

; The default installation directory
InstallDir $PROGRAMFILES\profimax\server

; Request application privileges for Windows Vista
RequestExecutionLevel admin

LoadLanguageFile "${NSISDIR}\Contrib\Language files\Russian.nlf"

;--------------------------------

; Pages

Page components
Page directory
Page custom nsInputUser nsGetInputUser
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles


Var PUSER
Var PPASS
Var iUSER
Var iPASS
Var iRPASS

Function nsGetInputUser
  ${NSD_GetText} $iUSER $PUSER
  ${NSD_GetText} $iPASS $PPASS
  ${NSD_GetText} $iRPASS $R0
  
  ${If} $PUSER == ""
      ${If} $PPASS == "" 
	  MessageBox MB_OK "Введите пароль и имя пользователя"
          Abort
      ${EndIf}
  ${EndIf}


  ${If} $PUSER == ""
	MessageBox MB_OK "Введите имя пользователя"
        Abort
  ${EndIf}
  
  ${If} $PPASS == ""
	MessageBox MB_OK "Введите пароль пользователя"
        Abort
  ${EndIf}

  ${If} $PPASS != $R0
	MessageBox MB_OK "Не совпадают введенные пароли"
        Abort
  ${EndIf}
  
FunctionEnd

Function nsInputUser
        

        nsDialogs::Create 1018
	Pop $0


        ${NSD_CreateLabel} 0% 1% 100% 24u "Пожалуйста введите имя пользователя и пароль администратора базы данных и пользователя который будет входить в качестве службы"
 	Pop $0


        ${NSD_CreateLabel} 0% 21% 50u 12u "Пользователь"
	Pop $0

	${NSD_CreateText} 20% 20% 80u 12u ""
	Pop $iUSER

	${NSD_CreateLabel} 9% 33% 25u 12u "Пароль"
 	Pop $0

        ${NSD_CreatePassword} 20% 32% 80u 12u ""
	Pop $iPASS
        
        ${NSD_CreateLabel} 6% 46% 35u 12u "Проверка"
 	Pop $0

        ${NSD_CreatePassword} 20% 45% 80u 12u ""
	Pop $iRPASS


	nsDialogs::Show

FunctionEnd


;--------------------------------

; The stuff to install
Section "ProfiMAX Server (обязательно)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  SetOverwrite on
  File /r *.*
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimaxserver" "DisplayName" "ProfiMAX Server"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimaxserver" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimaxserver" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimaxserver" "NoRepair" 1
  WriteUninstaller "uninstall.exe"


  nsExec::ExecToLog '"$INSTDIR\scripts\vcredist_x86.exe"'
  nsExec::ExecToLog '"$INSTDIR\scripts\createuser.exe" . $PUSER $PPASS'
  nsExec::ExecToLog 'sc create profimax displayname= "profimax" obj= ".\$PUSER" password= "$PPASS" start= auto binPath= "\"$INSTDIR\bin\pg_ctl.exe\" runservice -w -N profimax -D \"$INSTDIR\data\""'
  nsExec::ExecToLog 'cacls "$INSTDIR" /E /G $PUSER:F'
  nsExec::ExecToLog '"$INSTDIR\bin\initdb.exe" -D "$INSTDIR\data" --locale=Russian_Russia --encoding=UTF8 -U $PUSER'
  nsExec::ExecToLog 'net start profimax'
  nsExec::ExecToLog '"$INSTDIR\bin\createdb.exe" -U $PUSER profimax'  
  nsExec::ExecToLog '"$INSTDIR\bin\psql.exe" -U $PUSER -d profimax -f "$INSTDIR\scripts\basepostgre.sql"'
  nsExec::ExecToLog '"$INSTDIR\bin\pg_restore.exe" -O -d profimax -U $PUSER "$INSTDIR\scripts\basepostgre.backup"'
  nsExec::ExecToLog 'net stop profimax'
 
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Ярлыки в меню Пуск"

  CreateDirectory "$SMPROGRAMS\ProfiMAX"
  CreateShortCut "$SMPROGRAMS\ProfiMAX\Запустить ProfiMAX Server.lnk" "net" 'start profimax' "$INSTDIR\scripts\images\pg-start.ico" 0
  CreateShortCut "$SMPROGRAMS\ProfiMAX\Остановить ProfiMAX Server.lnk" "net" 'stop profimax' "$INSTDIR\scripts\images\pg-stop.ico" 0
  CreateShortCut "$SMPROGRAMS\ProfiMAX\pgAdmin III.lnk" "$INSTDIR\bin\pgAdmin3.exe" "" "$INSTDIR\bin\pgAdmin3.exe" 0
  CreateShortCut "$SMPROGRAMS\ProfiMAX\Удалить ProfiMAX Server.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimaxserver"
  DeleteRegKey HKLM SOFTWARE\profimaxserver

  nsExec::ExecToLog 'net stop profimax'
  nsExec::ExecToLog '$INSTDIR\bin\pg_ctl.exe unregister -N "profimax"'
  nsExec::ExecToLog 'net user /delete $PUSER'

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\ProfiMAX\Запустить ProfiMAX Server.lnk"
  Delete "$SMPROGRAMS\ProfiMAX\Остановить ProfiMAX Server.lnk"
  Delete "$SMPROGRAMS\ProfiMAX\Удалить ProfiMAX Server.lnk"
  Delete "$SMPROGRAMS\ProfiMAX\pgAdmin III.lnk"


  ; Remove directories used
  RMDir "$SMPROGRAMS\ProfiMAX"


  RMDir /r "$INSTDIR"

  RMDir "$INSTDIR\.."


SectionEnd
