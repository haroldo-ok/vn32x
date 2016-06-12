SETLOCAL
set PYTHON_HOME=c:\Python27
mkdir bin
del /q bin\*
call %PYTHON_HOME%\Scripts\cxfreeze apg.py --target-dir bin 
call %PYTHON_HOME%\Scripts\cxfreeze font_conv.py --target-dir bin 
call %PYTHON_HOME%\Scripts\cxfreeze rpy2c/rpy2c.py --target-dir bin
