//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Coordinate Conversion
//
//------------------------------------------------------
/// @file CoordinateConverter.cpp
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

#include <sstream>
#include <cmath>

#include "CoordinateConverter.hpp"

#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace CoordinateConversion
  {
    CoordinateConverter::CoordinateConverter()
    {
        m_ctx_p = proj_context_create();
        if (m_ctx_p == NULL) 
        { 
            throw EAGGRException("Failed to create proj context."); 
        }
        
        m_wgs84ToSphereCoordinateSystem_p = proj_create(
            m_ctx_p, "+proj=pipeline +step +inv +proj=longlat +datum=WGS84 +ellps=WGS84 +step +proj=longlat +datum=WGS84 +ellps=sphere");

        if (m_wgs84ToSphereCoordinateSystem_p == NULL) 
        { 
            throw EAGGRException("Failed to create WGS84 to sphere projection pipeline."); 
        }
    }

    CoordinateConverter::~CoordinateConverter()
    {
        if (m_wgs84ToSphereCoordinateSystem_p != NULL) 
        { 
            proj_destroy(m_wgs84ToSphereCoordinateSystem_p); 
        }

        if (m_ctx_p != NULL) 
        { 
            proj_context_destroy(m_ctx_p); 
        }
    }

    EAGGR::LatLong::SphericalAccuracyPoint CoordinateConverter::ConvertWGS84ToSphere(
        const EAGGR::LatLong::Wgs84AccuracyPoint a_wgs84Point) const
    { 
      PJ_COORD c;        
      // Initially assume a height of zero and perform the conversion
      c.lpzt.z     = 0.0;
      c.lpzt.t     = HUGE_VAL;
      c.lpzt.lam   = a_wgs84Point.GetLongitudeInRadians();
      c.lpzt.phi   = a_wgs84Point.GetLatitudeInRadians();

      int returnCode = proj_trans_array(m_wgs84ToSphereCoordinateSystem_p,
                                        PJ_FWD,
                                        m_NUM_POINTS_TO_CONVERT,
                                        &c);

      if (returnCode != 0)
      {
        const char* error = proj_errno_string(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      // First conversion provides a height offset with an offset in the latitude/longitude values
      // Need to perform the conversion again on the original lat/long using the negated height offset
      // This provides the correct lat/long to a good approximation.
      c.lpzt.z = -c.lpzt.z;
      c.lpzt.lam = a_wgs84Point.GetLongitudeInRadians();
      c.lpzt.phi = a_wgs84Point.GetLatitudeInRadians();

      returnCode = proj_trans_array(m_wgs84ToSphereCoordinateSystem_p, PJ_FWD, m_NUM_POINTS_TO_CONVERT, &c);

      if (returnCode != 0)
      {
        const char* error = proj_errno_string(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      const double latitude = RADIANS_IN_DEG(c.xy.y);
      // To maintain previous behaviour of proj4 set longitdue on the poles to be zero
      const double longitude = latitude == 90.0 || latitude == -90.0 ? 0.0 : RADIANS_IN_DEG(c.xy.x);
      return EAGGR::LatLong::SphericalAccuracyPoint(
          latitude,
          longitude,
          EAGGR::LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(
              a_wgs84Point.GetAccuracy()));
    }

    EAGGR::LatLong::Wgs84AccuracyPoint CoordinateConverter::ConvertSphereToWGS84(
        const EAGGR::LatLong::SphericalAccuracyPoint a_spherePoint) const
    {
      PJ_COORD c;
      c.lpzt.z = 0.0;
      c.lpzt.t = HUGE_VAL;
      c.lpzt.lam = a_spherePoint.GetLongitudeInRadians();
      c.lpzt.phi = a_spherePoint.GetLatitudeInRadians();

      int returnCode = proj_trans_array(m_wgs84ToSphereCoordinateSystem_p, PJ_INV, m_NUM_POINTS_TO_CONVERT, &c);

      if (returnCode != 0)
      {
        const char* error = proj_errno_string(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      const double latitude = RADIANS_IN_DEG(c.xy.y);
      // To maintain previous behaviour of proj4 set longitdue on the poles to be zero
      const double longitude = latitude == 90.0 || latitude == -90.0 ? 0.0 : RADIANS_IN_DEG(c.xy.x);
      return EAGGR::LatLong::Wgs84AccuracyPoint(
          latitude,
          longitude,
          EAGGR::LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(
              a_spherePoint.GetAccuracy()));
    }
  }
}
