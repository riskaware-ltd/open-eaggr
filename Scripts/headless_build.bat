@ECHO OFF
REM headless_build.bat - Runs Eclipse in headless mode to build the OpenEAGGR DLL and test harness.
REM Takes one input argument which is the name of the configuration to build.

cd ..

eclipse.exe --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data .\Scripts\temp_workspace -import ".\EAGGR" -cleanBuild EAGGR/%1
IF ERRORLEVEL 1 GOTO error_handling

eclipse.exe --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data .\Scripts\temp_workspace -import ".\EAGGRTestHarness" -cleanBuild EAGGRTestHarness/%1
IF ERRORLEVEL 1 GOTO error_handling

eclipse.exe --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data .\Scripts\temp_workspace -import ".\EAGGRPostgres" -cleanBuild EAGGRPostgres/%1
IF ERRORLEVEL 1 GOTO error_handling

eclipse.exe --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data .\Scripts\temp_workspace -import ".\EAGGRPostgresTestHarness" -cleanBuild EAGGRPostgresTestHarness/%1
IF ERRORLEVEL 1 GOTO error_handling

ECHO Successfully built source
PAUSE
EXIT 0

:error_handling
ECHO Failed to build source (Exit code = %ERRORLEVEL%)
PAUSE
EXIT ERRORLEVEL
