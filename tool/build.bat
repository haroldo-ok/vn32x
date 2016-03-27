SETLOCAL
set PYTHON_HOME=c:\Python27
mkdir bin
del /q bin\*
call %PYTHON_HOME%\Scripts\cxfreeze apg.py --target-dir bin --include-path=tw;lib
call %PYTHON_HOME%\Scripts\cxfreeze font_conv.py --target-dir bin --include-path=tw;lib
