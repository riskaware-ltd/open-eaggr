#!/bin/bash
# Packages the EAGGR Python Library into a wheel (.whl) file.
# Important: Must be run from the EAGGRPython directory.

# Copy all windows dlls for wheel
cp -v ../EAGGR/3rdParty/bin/win32-x86/libgdal-20.dll eaggr/dlls/win32-x86/
cp -v ../EAGGR/3rdParty/bin/win32-x86-64/libgdal-20.dll eaggr/dlls/win32-x86-64/
cp -v ../EAGGR/3rdParty/bin/win32-x86/libproj-9.dll eaggr/dlls/win32-x86/
cp -v ../EAGGR/3rdParty/bin/win32-x86-64/libproj-9.dll eaggr/dlls/win32-x86-64/

python2.7 setup.py bdist_wheel --universal
