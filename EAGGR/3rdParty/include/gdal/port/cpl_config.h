//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: 3rd Party
//
//------------------------------------------------------
/// @file cpl_config.h
///
/// Header to allow GDAL code to be used for both Windows and Linux.
//------------------------------------------------------

/* This file simply chooses the correct cpl_config.h file to include out of
   config_linux.h and cpl_config_win32.h . If you re-install gdal you will 
   likely overwrite this file - which shouldn't be a problem on *this* platform,
   but will break if you check it in and use it on other platforms. */

/* If neither of these is true your compile will fail */
#ifdef _WIN32
#  include "cpl_config_win32.h"
#elif __GNUC__ >= 4
#  include "cpl_config_linux.h"
#else 
#  error Requires implementation for target platform 
#endif

