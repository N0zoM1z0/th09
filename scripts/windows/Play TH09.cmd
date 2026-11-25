@echo off
setlocal
set "__COMPAT_LAYER=DISABLEDXMAXIMIZEDWINDOWEDMODE"
start "" /d "%~dp0." "%~dp0th09-reconstructed.exe"
