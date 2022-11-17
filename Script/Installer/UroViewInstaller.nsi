; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "UroVIew"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "GTSM"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\UroVIew.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
BrandingText " "
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "AVMDlicense.txt"
; Directory page
;!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\UroVIew.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Korean"
; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "C:\UroView\"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "C:\UroView\"
  SetOverwrite ifnewer
  File "D:\Project\uro\URO\Work\UroView\Runtime\bin\Release\UroView.exe"
  CreateDirectory "$SMPROGRAMS\UroView"
  CreateShortCut "$SMPROGRAMS\UroView\UroView.lnk" "$INSTDIR\UroView.exe"
  CreateShortCut "$DESKTOP\UroView.lnk" "$INSTDIR\AVMDMonitor32.exe"

SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\UroView\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  Call CheckAndInstallDotNet
  Call InstallVCRedist
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\UroView.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\UroView.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Function InstallVCRedist
    Call CheckVCRedist
    Pop $R0
    StrCmp $R0 "No" 0 +3
    File "D:\Project\SetUp\vcredist_x86.exe"
   ExecWait '"vcredist_x86.exe" /q:a /c:"msiexec /i vcredist.msi /qb! /l*v $TEMP\vcredist_x86.log" ' # suppress all UI during installation.

FunctionEnd


Function CheckVCRedist
   Push $R0
   ClearErrors
   ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{{9A25302D-30C0-39D9-BD6F-21E6EC160475}}" "Version"

   ; if VS 2005+ redist SP1 not installed, install it
   IfErrors 0 VSRedistInstalled
   StrCpy $R0 "No"

VSRedistInstalled:
   Exch $R0
FunctionEnd

Function CheckAndInstallDotNet
    ; Magic numbers from http://msdn.microsoft.com/en-us/library/ee942965.aspx
    ClearErrors
    ReadRegDWORD $0 HKLM "SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full" "Release"

    IfErrors NotDetected

    ${If} $0 >= 378389

        DetailPrint "Microsoft .NET Framework 4.5 is installed ($0)"
    ${Else}
    NotDetected:
        DetailPrint "Installing Microsoft .NET Framework 4.5"
        SetDetailsPrint listonly
        ExecWait '"$INSTDIR\Tools\dotNetFx45_Full_setup.exe" /passive /norestart' $0
        ${If} $0 == 3010
        ${OrIf} $0 == 1641
            DetailPrint "Microsoft .NET Framework 4.5 installer requested reboot"
            SetRebootFlag true
        ${EndIf}
        SetDetailsPrint lastused
        DetailPrint "Microsoft .NET Framework 4.5 installer returned $0"
    ${EndIf}

FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name)는(은) 완전히 제거되었습니다."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(^Name)을(를) 제거하시겠습니까?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd