# headless_build.sh - Runs Eclipse in headless mode to build the EAGGR DLL and test harness.

cd ..

if ! eclipse --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data ./Scripts/temp_workspace -import "./EAGGRLinux" -cleanBuild all ; then
    echo "Failed to build EAGGR source"
    exit 1
fi

if ! eclipse --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data ./Scripts/temp_workspace -import "./EAGGRTestHarnessLinux" -cleanBuild all ; then
    echo "Failed to build EAGGRTestHarness source"
    exit 1
fi

echo "Successfully built source"

