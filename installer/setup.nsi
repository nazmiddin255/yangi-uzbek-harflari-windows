; =====================================================================
;  "Uzbek - yangi alifbo (AltGr)" klaviatura o'rnatuvchisi
;  Yig'ish: ../build.sh  (yoki: makensis -DVER=1.0.2 -DOUTDIR=../dist setup.nsi)
; =====================================================================
Unicode true
!include "MUI2.nsh"
!include "x64.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"

!ifndef VER
  !define VER "0.0.0"
!endif
!ifndef OUTDIR
  !define OUTDIR "..\dist"
!endif

!define NAME    "Uzbek yangi alifbo AltGr"
!define LAYOUT  "Uzbek - yangi alifbo (AltGr)"
!define KLID    "a0de0443"                ; registry'dagi klaviatura raqami
!define LAYOUTID "0a7c"                   ; tizimda yagona bo'lishi kerak
!define TIP     "0443:A0DE0443"           ; til ro'yxatidagi yozuv
!define KLKEY   "SYSTEM\CurrentControlSet\Control\Keyboard Layouts\${KLID}"
!define UNKEY   "Software\Microsoft\Windows\CurrentVersion\Uninstall\UzbekYangiAlifboAltGr"
!define URL     "https://github.com/nazmiddin255/yangi-uzbek-harflari"
!define PS      '"$WINDIR\Sysnative\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -Command'

Name "${NAME}"
Caption "${NAME} ${VER}"
OutFile "${OUTDIR}\uzbek-yangi-alifbo-setup-${VER}.exe"
InstallDir "$PROGRAMFILES64\${NAME}"
RequestExecutionLevel admin
SetCompressor /SOLID lzma
BrandingText "${NAME} ${VER}  -  MIT License"

VIProductVersion "${VER}.0"
VIAddVersionKey /LANG=1033 "ProductName"     "${NAME}"
VIAddVersionKey /LANG=1033 "ProductVersion"  "${VER}"
VIAddVersionKey /LANG=1033 "FileVersion"     "${VER}"
VIAddVersionKey /LANG=1033 "FileDescription" "${NAME} - klaviatura o'rnatuvchisi"
VIAddVersionKey /LANG=1033 "CompanyName"     "nazmiddin255 (GitHub)"
VIAddVersionKey /LANG=1033 "LegalCopyright"  "MIT License"

!define MUI_ICON   "icon.ico"
!define MUI_UNICON "icon.ico"
!define MUI_ABORTWARNING

!define MUI_WELCOMEPAGE_TITLE "${LAYOUT}"
!define MUI_WELCOMEPAGE_TEXT "Bu dastur Windows'ga yangi klaviatura qo'shadi.$\r$\n$\r$\nO'ng Alt (AltGr) + S / C / O / G  ->  ş ç ö ğ$\r$\nAltGr + Shift + S / C / O / G  ->  Ş Ç Ö Ğ$\r$\nAltGr + `  ->  ʼ (tutuq belgisi)$\r$\n$\r$\nQolgan barcha tugmalar oddiy US klaviaturadek ishlaydi.$\r$\n$\r$\nBu norasmiy, ochiq kodli loyiha. Dastur 'boricha' (as is) beriladi."
!define MUI_FINISHPAGE_TITLE "Tayyor!"
!define MUI_FINISHPAGE_TEXT "Kompyuterni qayta yuklang.$\r$\n$\r$\nKeyin Win + Space ni bosib, '${LAYOUT}' ni tanlang.$\r$\n$\r$\nAgar Windows katta yangilanishdan keyin klaviatura yo'qolsa, shu o'rnatuvchini qayta ishga tushiring."
!define MUI_FINISHPAGE_LINK "Loyiha sahifasi (GitHub)"
!define MUI_FINISHPAGE_LINK_LOCATION "${URL}"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "Uzbek"

; ---------------------------------------------------------------------
Function .onInit
  ${IfNot} ${AtLeastWin10}
    MessageBox MB_ICONSTOP "Bu paket Windows 10 yoki 11 uchun."
    Abort
  ${EndIf}
  ${IfNot} ${IsNativeAMD64}
    MessageBox MB_ICONSTOP "Bu paket faqat 64-bitli (x64) Windows uchun.$\r$\n32-bitli va ARM kompyuterlar hozircha qo'llab-quvvatlanmaydi."
    Abort
  ${EndIf}
  SetRegView 64
FunctionEnd

Function un.onInit
  SetRegView 64
FunctionEnd

; Eski DLL band bo'lsa (ishlatilayotgan bo'lsa), nomini .old ga o'zgartirib,
; qayta yuklanganda o'chiriladigan qilamiz va yangisini yozamiz.
!macro PutDll SUBDIR SRC
  Delete "$WINDIR\${SUBDIR}\uzaltgr.dll.old"
  IfFileExists "$WINDIR\${SUBDIR}\uzaltgr.dll" 0 +3
    Rename "$WINDIR\${SUBDIR}\uzaltgr.dll" "$WINDIR\${SUBDIR}\uzaltgr.dll.old"
    Delete /REBOOTOK "$WINDIR\${SUBDIR}\uzaltgr.dll.old"
  SetOutPath "$WINDIR\${SUBDIR}"
  File "${SRC}"
!macroend

Section "Install"
  DetailPrint "Klaviatura fayllari nusxalanmoqda..."
  ${DisableX64FSRedirection}
  !insertmacro PutDll "System32" "${OUTDIR}\x64\uzaltgr.dll"
  ${EnableX64FSRedirection}
  !insertmacro PutDll "SysWOW64" "${OUTDIR}\x86\uzaltgr.dll"

  DetailPrint "Klaviatura ro'yxatdan o'tkazilmoqda..."
  WriteRegStr HKLM "${KLKEY}" "Layout File" "uzaltgr.dll"
  WriteRegStr HKLM "${KLKEY}" "Layout Text" "${LAYOUT}"
  WriteRegStr HKLM "${KLKEY}" "Layout Id"   "${LAYOUTID}"

  DetailPrint "Til ro'yxatiga qo'shilmoqda (O'zbek, lotin)..."
  nsExec::ExecToLog '${PS} "$$l = Get-WinUserLanguageList; $$u = $$l | Where-Object { $$_.LanguageTag -like $\'uz-Latn*$\' } | Select-Object -First 1; if (-not $$u) { $$l.Add($\'uz-Latn-UZ$\'); $$u = $$l | Where-Object { $$_.LanguageTag -like $\'uz-Latn*$\' } | Select-Object -First 1 }; if ($$u.InputMethodTips -notcontains $\'${TIP}$\') { $$u.InputMethodTips.Add($\'${TIP}$\') }; Set-WinUserLanguageList $$l -Force"'
  Pop $0

  SetOutPath "$INSTDIR"
  File "..\LICENSE"
  File "icon.ico"
  WriteUninstaller "$INSTDIR\uninstall.exe"

  ; "Installed apps" ro'yxatidagi yozuv
  WriteRegStr   HKLM "${UNKEY}" "DisplayName"     "${NAME}"
  WriteRegStr   HKLM "${UNKEY}" "DisplayVersion"  "${VER}"
  WriteRegStr   HKLM "${UNKEY}" "DisplayIcon"     "$INSTDIR\icon.ico"
  WriteRegStr   HKLM "${UNKEY}" "Publisher"       "nazmiddin255 (GitHub)"
  WriteRegStr   HKLM "${UNKEY}" "URLInfoAbout"    "${URL}"
  WriteRegStr   HKLM "${UNKEY}" "InstallLocation" "$INSTDIR"
  WriteRegStr   HKLM "${UNKEY}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr   HKLM "${UNKEY}" "QuietUninstallString" '"$INSTDIR\uninstall.exe" /S'
  WriteRegDWORD HKLM "${UNKEY}" "EstimatedSize"   60
  WriteRegDWORD HKLM "${UNKEY}" "NoModify" 1
  WriteRegDWORD HKLM "${UNKEY}" "NoRepair" 1
  SetRebootFlag true
SectionEnd

Section "Uninstall"
  DetailPrint "Til ro'yxatidan olib tashlanmoqda..."
  nsExec::ExecToLog '${PS} "$$l = Get-WinUserLanguageList; foreach ($$x in $$l) { [void]$$x.InputMethodTips.Remove($\'${TIP}$\') }; Set-WinUserLanguageList $$l -Force"'
  Pop $0
  DeleteRegKey HKLM "${KLKEY}"
  ${DisableX64FSRedirection}
  Delete /REBOOTOK "$WINDIR\System32\uzaltgr.dll"
  ${EnableX64FSRedirection}
  Delete /REBOOTOK "$WINDIR\SysWOW64\uzaltgr.dll"
  DeleteRegKey HKLM "${UNKEY}"
  Delete "$INSTDIR\LICENSE"
  Delete "$INSTDIR\icon.ico"
  Delete "$INSTDIR\uninstall.exe"
  RMDir "$INSTDIR"
  SetRebootFlag true
SectionEnd
