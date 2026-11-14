@echo off
setlocal EnableExtensions
cd /d "%~dp0"
echo ==================================================
echo  HPJ Inventory - Portable Windows EXE Builder
echo ==================================================

where cl.exe >nul 2>nul
if errorlevel 1 (
  echo ERROR: Visual Studio C++ Build Tools are not available in this terminal.
  echo Open "x64 Native Tools Command Prompt for VS 2022" and run this file again.
  pause
  exit /b 1
)

if not exist nuget.exe (
  echo Downloading NuGet command line...
  powershell -NoProfile -ExecutionPolicy Bypass -Command "Invoke-WebRequest 'https://dist.nuget.org/win-x86-commandline/latest/nuget.exe' -OutFile 'nuget.exe'"
  if errorlevel 1 goto :fail
)

if not exist packages\Microsoft.Web.WebView2.1.0.2903.40\build\native\include\WebView2.h (
  echo Downloading Microsoft WebView2 SDK...
  nuget.exe install Microsoft.Web.WebView2 -Version 1.0.2903.40 -OutputDirectory packages -NonInteractive
  if errorlevel 1 goto :fail
)

set SDK=packages\Microsoft.Web.WebView2.1.0.2903.40\build\native
if not exist "%SDK%\include\WebView2.h" goto :fail
if not exist "%SDK%\x64\WebView2LoaderStatic.lib" (
  echo ERROR: WebView2 static loader library not found in the SDK package.
  goto :fail
)

if not exist output mkdir output
rc /nologo /fo output\app.res app.rc
if errorlevel 1 goto :fail
cl /nologo /O1 /GL /MT /EHsc /std:c++17 /DUNICODE /D_UNICODE /I "%SDK%\include" main.cpp output\app.res /link /LTCG /OPT:REF /OPT:ICF /SUBSYSTEM:WINDOWS /LIBPATH:"%SDK%\x64" WebView2LoaderStatic.lib ole32.lib shell32.lib shlwapi.lib user32.lib gdi32.lib /OUT:output\HPJ_Inventory.exe
if errorlevel 1 goto :fail

copy /y Inventory.html output\Inventory.html >nul
copy /y HPJ.ico output\HPJ.ico >nul
if not exist output\HPJ_Data mkdir output\HPJ_Data
(
  echo HPJ Inventory Portable
  echo Keep HPJ_Inventory.exe and Inventory.html together in this folder.
  echo Data is stored in the HPJ_Data folder beside the application.
  echo Microsoft Edge WebView2 Evergreen Runtime is required on the target PC.
) > output\README.txt

echo.
echo BUILD SUCCESSFUL.
echo Portable application folder: %CD%\output
echo Copy the complete output folder to a USB drive or another PC.
echo The target PC needs Microsoft Edge WebView2 Runtime; no app installation is required.
echo.
pause
exit /b 0

:fail
echo.
echo BUILD FAILED. Check the messages above.
pause
exit /b 1
