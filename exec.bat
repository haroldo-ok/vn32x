@Echo Off

@REM Set this according to your own environment setup
SET DEVKITROOT=C:\Util\dev32x
SET PACKFIREROOT=C:\Util\sixpack-13
SET TOOLROOT=%~dp0\tool

Set PATH=%DEVKITROOT%\bin;%DEVKITROOT%\sh-elf\bin;%DEVKITROOT%\m68k-elf\bin;%DEVKITROOT%\UTILS;%PACKFIREROOT%\bin;%TOOLROOT%\bin;%PATH%
Set MAKE_MODE=unix

cd %~dp0
%1 %2 %3 %4 %5 %6 %7
