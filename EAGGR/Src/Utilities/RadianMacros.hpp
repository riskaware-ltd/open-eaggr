//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Utilities
//
//------------------------------------------------------
/// @file RadianMacros.hpp
/// 
/// Macros for working in Radians.
///
/// @warning Because this file contains macros it should
///          not be included in any header files.
///
/// This file is part of OpenEAGGR.
///
/// OpenEAGGR is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenEAGGR is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Lesser General Public License for more details.
///
/// A copy of the GNU Lesser General Public License is available in COPYING.LESSER
/// or can be found at <http://www.gnu.org/licenses/>.
//------------------------------------------------------

#pragma once

/// Value for pi.
#define PI 3.14159265358979323846

/// Macro for converting angles in degrees to radians.
#define DEGREES_IN_RAD(degrees) (degrees * PI / 180.0)

/// Macro for converting angles in radians to degrees.
#define RADIANS_IN_DEG(radians) (radians * 180.0 / PI)
