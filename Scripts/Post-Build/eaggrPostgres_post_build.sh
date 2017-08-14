#!/bin/bash
#
# Post build actions for the EAGGRPostgres project

#==========================================

# Constants
architecture="linux-x86"
project="eaggr_postgres"

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
    dll_name="lib_"${project}"_d"
else
    target_dir_prefix="Release"
    dll_name="lib_"${project}
fi

if [[ $platform == "64" ]]; then
    dll_dir=${architecture}"-64"
    target_dir=${target_dir_prefix}"64"
else
    dll_dir=${architecture}"-32"
    target_dir=${target_dir_prefix}
fi


# Copy postgres extension library and dependencies to the deploy directory
mkdir -p ../../EAGGRPostgresLinux/deploy/
cp -av ../../EAGGRPostgresLinux/${target_dir}/libEAGGRPostgresLinux ../../EAGGRPostgresLinux/deploy/lib_eaggr_postgres.so
cp -av ../../EAGGRLinux/${target_dir}/libeaggr.so ../../EAGGRPostgresLinux/deploy/
