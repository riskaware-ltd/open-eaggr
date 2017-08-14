@ECHO OFF
REM Post build actions for the EAGGR project.

REM Get arguments
SET build=%1
SET platform=%2

REM Determine folder names based on build type and target platform
IF "%platform%" == "64" (
  SET dll_dir="win32-x86-64"
  SET lib_gcc_dll="LIBGCC_S_SEH-1"
  IF "%build%" == "debug" (
    SET target_dir="Debug64"
  ) ELSE (
    SET target_dir="Release64"
  )
) ELSE (
  SET dll_dir="win32-x86"
  SET lib_gcc_dll="LIBGCC_S_DW2-1"
  IF "%build%" == "debug" (
    SET target_dir="Debug"
  ) ELSE (
    SET target_dir="Release"
  )
)

REM Determine name of EAGGR DLL
IF "%build%" == "debug" (
  SET dll_name="eaggr_d"
) ELSE (
  SET dll_name="eaggr"
)

REM Get the Geospatial Data Abstraction Library
XCOPY /Y ..\3rdParty\bin\%dll_dir%\libgdal-20.dll .\

REM Get the Proj4 library
XCOPY /Y ..\3rdParty\bin\%dll_dir%\libproj-9.dll .\

REM Make sure the target directory for the Test Harness exists
IF NOT EXIST ..\..\EAGGRTestHarness\%target_dir%\ (
  MKDIR ..\..\EAGGRTestHarness\%target_dir%\
)

REM Make sure the target directory for EAGGRPostgres exists
IF NOT EXIST ..\..\EAGGRPostgres\deploy\%target_dir%\ (
  MKDIR ..\..\EAGGRPostgres\deploy\%target_dir%\
)

REM Copy the new EAGGR DLL to the test harness
XCOPY /Y %dll_name%.dll ..\..\EAGGRTestHarness\%target_dir%\
XCOPY /Y libgdal-20.dll ..\..\EAGGRTestHarness\%target_dir%\
XCOPY /Y libproj-9.dll ..\..\EAGGRTestHarness\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\%lib_gcc_dll%.DLL ..\..\EAGGRTestHarness\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBSTDC++-6.DLL ..\..\EAGGRTestHarness\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBWINPTHREAD-1.DLL ..\..\EAGGRTestHarness\%target_dir%\

REM Copy the new EAGGR DLL and dependencies to the Java API project
XCOPY /Y %dll_name%.dll ..\..\EAGGRJava\dlls\%dll_dir%\
XCOPY /Y libgdal-20.dll ..\..\EAGGRJava\dlls\%dll_dir%\
XCOPY /Y libproj-9.dll ..\..\EAGGRJava\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\%lib_gcc_dll%.DLL ..\..\EAGGRJava\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBSTDC++-6.DLL ..\..\EAGGRJava\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBWINPTHREAD-1.DLL ..\..\EAGGRJava\dlls\%dll_dir%\

REM Copy the new EAGGR DLL and dependencies to the Python API project
XCOPY /Y %dll_name%.dll ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\
XCOPY /Y libgdal-20.dll ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\
XCOPY /Y libproj-9.dll ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\%lib_gcc_dll%.DLL ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBSTDC++-6.DLL ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBWINPTHREAD-1.DLL ..\..\EAGGRPython\eaggr\dlls\%dll_dir%\

REM Copy the new EAGGR DLL to the Postgres project
XCOPY /Y %dll_name%.dll ..\..\EAGGRPostgres\deploy\%target_dir%\
XCOPY /Y libgdal-20.dll ..\..\EAGGRPostgres\deploy\%target_dir%\
XCOPY /Y libproj-9.dll ..\..\EAGGRPostgres\deploy\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\%lib_gcc_dll%.DLL ..\..\EAGGRPostgres\deploy\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBSTDC++-6.DLL ..\..\EAGGRPostgres\deploy\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBWINPTHREAD-1.DLL ..\..\EAGGRPostgres\deploy\%target_dir%\
