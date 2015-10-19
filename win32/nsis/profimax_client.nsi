; profimax.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install profimax.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "ProfiMAX"

; The file to write
OutFile "..\profimax_client.exe"

; The default installation directory
InstallDir $PROGRAMFILES\profimax\client

; Request application privileges for Windows Vista
RequestExecutionLevel admin

LoadLanguageFile "${NSISDIR}\Contrib\Language files\Russian.nlf"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "ProfiMAX (обязательно)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  SetOverwrite on
  File /r *.*
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimax" "DisplayName" "ProfiMAX"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimax" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimax" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimax" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Ярлыки в меню Пуск"

  CreateDirectory "$SMPROGRAMS\ProfiMAX"
  CreateShortCut "$SMPROGRAMS\ProfiMAX\ProfiMAX.lnk" "$INSTDIR\profimax.exe" "" "$INSTDIR\profimax.exe" 0
  CreateShortCut "$SMPROGRAMS\ProfiMAX\Удалить ProfiMAX.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\profimax"
  DeleteRegKey HKLM SOFTWARE\profimax


  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\ProfiMAX\ProfiMAX.lnk"
  Delete "$SMPROGRAMS\ProfiMAX\Удалить ProfiMAX.lnk"

  ; Remove directories used
   RMDir "$SMPROGRAMS\ProfiMAX"


  RMDir /r "$INSTDIR"

  RMDir "$INSTDIR\.."
  
  

SectionEnd
