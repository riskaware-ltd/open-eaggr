@ECHO OFF
REM Post build actions for the EAGGRPostgres project.

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

REM Make sure the target directory for EAGGRPostgres exists
IF NOT EXIST ..\..\EAGGRPostgres\deploy\%target_dir%\ (
  MKDIR ..\..\EAGGRPostgres\deploy\%target_dir%\
)

REM Make sure the target directory for the EAGGRPostgres Test Harness exists
IF NOT EXIST ..\..\EAGGRPostgresTestHarness\%target_dir%\ (
  MKDIR ..\..\EAGGRPostgresTestHarness\%target_dir%\
)

REM Copy the new EAGGRPostgres DLL to the EAGGRPostgres project
IF "%build%" == "debug" (
	XCOPY /Y ..\..\EAGGRPostgres\%target_dir%\lib_eaggr_postgres_d.dll ..\..\EAGGRPostgres\deploy\%target_dir%\
) ELSE (
	XCOPY /Y ..\..\EAGGRPostgres\%target_dir%\lib_eaggr_postgres.dll ..\..\EAGGRPostgres\deploy\%target_dir%\
)

REM Copy dependencies of test harness
XCOPY /Y %MINGW_HOME%\bin\%lib_gcc_dll%.DLL ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBSTDC++-6.DLL ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y %MINGW_HOME%\bin\LIBWINPTHREAD-1.DLL ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y ..\..\EAGGRPostgres\3rdParty\bin\libeay32.dll ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y ..\..\EAGGRPostgres\3rdParty\bin\libiconv-2.dll ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y ..\..\EAGGRPostgres\3rdParty\bin\libintl-8.dll ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y ..\..\EAGGRPostgres\3rdParty\bin\libpq.dll ..\..\EAGGRPostgresTestHarness\%target_dir%\
XCOPY /Y ..\..\EAGGRPostgres\3rdParty\bin\ssleay32.dll ..\..\EAGGRPostgresTestHarness\%target_dir%\
