@echo off
setlocal
set "__COMPAT_LAYER=DISABLEDXMAXIMIZEDWINDOWEDMODE"
"%~dp0th09-practice.exe"
if errorlevel 1 pause
