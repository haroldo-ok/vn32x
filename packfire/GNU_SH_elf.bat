@Echo Off

@REM Set this according to your own environment setup
SET DEVKITROOT=C:\Util\dev32x
SET PACKFIREROOT=C:\Util\sixpack-13

Set PATH=%DEVKITROOT%\bin;%DEVKITROOT%\sh-elf\bin;%DEVKITROOT%\m68k-elf\bin;%DEVKITROOT%\UTILS;%PACKFIREROOT%\bin;%PATH%
Set MAKE_MODE=unix

Cmd "cd\" /k
