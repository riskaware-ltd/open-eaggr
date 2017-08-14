@ECHO OFF
REM Packages the EAGGR Python Library into a the wheel (.whl) file.
REM Important: Must be run from the EAGGRPython directory.

set PYTHONPATH=C:\Python27;C:\Python27\DLLs;C:\Python27\Lib;C:\Python27\lib\site-packages
C:\Python27\python.exe setup.py bdist_wheel --universal
