//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Coordinate Conversion
//
//------------------------------------------------------
/// @file CoordinateConverter.hpp
/// 
/// Implements the EAGGR::CoordinateConversion::CoordinateConverter class.
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

#include "proj.h"

#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"

namespace EAGGR
{
  namespace CoordinateConversion
  {
    /// Class to provide ability to convert between WGS84 and Spherical Earth coordinates.
    class CoordinateConverter
    {
      public:
        /// Constructor
        CoordinateConverter();

        /// Destructor
        ///
        /// Frees the memory allocated to the member variables by proj4
        ~CoordinateConverter();

        /// Converts a point in WGS84 to Spherical Earth coordinates.
        /// @param a_wgs84Point The WGS84 point to transform.
        /// @return The Spherical Earth coordinate.
        EAGGR::LatLong::SphericalAccuracyPoint ConvertWGS84ToSphere(
            const EAGGR::LatLong::Wgs84AccuracyPoint a_wgs84Point) const;

        /// Converts a point in Spherical Earth coordinates to WGS84.
        /// @param a_spherePoint The Spherical Earth point to transform.
        /// @return The WGS84 coordinate.
        EAGGR::LatLong::Wgs84AccuracyPoint ConvertSphereToWGS84(
            const EAGGR::LatLong::SphericalAccuracyPoint a_spherePoint) const;

      private:
        // Prevent copying of class to prevent destructor freeing the projection members
        CoordinateConverter(const CoordinateConverter&);
        CoordinateConverter& operator=(const CoordinateConverter&);

        static const long m_NUM_POINTS_TO_CONVERT = 1;
        static const int m_POINT_OFFSET = 1;

        PJ* m_wgs84ToSphereCoordinateSystem_p;
        PJ_CONTEXT* m_ctx_p;
    };
  }
}
