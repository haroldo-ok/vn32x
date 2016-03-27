mkdir bin
del /q bin\*
c:\Python27\Scripts\cxfreeze apg.py --target-dir bin --include-path=tw;lib
