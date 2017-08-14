@ECHO OFF
REM Uninstalls the EAGGR Python Library package (for Python 2.7 and 3.4).

REM "--yes" argument used to bypass confirmation to uninstall the packages
set PYTHONPATH=C:\Python27;C:\Python27\DLLs;C:\Python27\Lib;C:\Python27\lib\site-packages
C:\Python27\Scripts\pip.exe uninstall --yes eaggr
set PYTHONPATH=C:\Python34;C:\Python34\DLLs;C:\Python34\Lib;C:\Python34\lib\site-packages
C:\Python34\Scripts\pip.exe uninstall --yes eaggr
