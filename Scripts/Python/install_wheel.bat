@ECHO OFF
REM Installs the EAGGR Python Library wheel (.whl) file (for Python 2.7 and 3.4).
REM Important: Must be run from the EAGGRPython directory.

set PYTHONPATH=C:\Python27;C:\Python27\DLLs;C:\Python27\Lib;C:\Python27\lib\site-packages
C:\Python27\Scripts\pip.exe install dist\eaggr-2.0-py2.py3-none-any.whl
set PYTHONPATH=C:\Python34;C:\Python34\DLLs;C:\Python34\Lib;C:\Python34\lib\site-packages
C:\Python34\Scripts\pip.exe install dist\eaggr-2.0-py2.py3-none-any.whl
