#!/bin/bash
#
# Post build actions for the EAGGR project

#==========================================
# Function:	copy_files <destination>
# Purpose:	Copies a set of files, make sure destination exists
#------------------------------------------
copy_files() {

    echo "Copying libraries to $1"

# Make sure the target directory exists
    mkdir -p "$1"

    cp -v ${dll_name}.so "$1"

# Get the Geospatial Data Abstraction Library
    cp -av ../3rdParty/bin/${dll_dir}/libgdal.so* "$1"

# Get the Proj4 library
    cp -av ../3rdParty/bin/${dll_dir}/libproj.so* "$1"
}
#------------------------------------------

# Constants
architecture="linux-x86"
project="eaggr"

# First test args
if [ $# -lt 2 ]; then
    echo "Incorrect args specified. Should be: <debug|release> <64|32>"
    exit 1
fi

# Get arguments
build=$1
platform=$2

# Determine folder/file names based on build type and target platform

if [[ ${build^^} == "DEBUG" ]]; then
    target_dir_prefix="Debug"
    dll_name="lib"${project}"_d"
else
    target_dir_prefix="Release"
    dll_name="lib"${project}
fi

if [[ $platform == "64" ]]; then
    dll_dir=${architecture}"-64"
    target_dir=${target_dir_prefix}"64"
else
    dll_dir=${architecture}"-32"
    target_dir=${target_dir_prefix}
fi

# Copy the new EAGGR DLL to the test harness
copy_files "../../EAGGRTestHarnessLinux/${target_dir}/"

# Copy the new EAGGR DLL and dependencies to the Java API project
copy_files "../../EAGGRJava/dlls/${dll_dir}/"

# Copy the new EAGGR DLL and dependencies to the Python API project
copy_files "../../EAGGRPython/eaggr/dlls/${dll_dir}/"

# Copy the new EAGGR DLL and dependencies to the EAGGRPostgres project
copy_files "../../EAGGRPostgres/deploy/${dll_dir}/"
