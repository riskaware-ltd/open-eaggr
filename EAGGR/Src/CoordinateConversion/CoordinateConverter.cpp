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
        m_wgs84CoordinateSystem_p = proj_create_crs_to_crs(PJ_DEFAULT_CTX,
                                                       "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs",
                                                       "+proj=longlat +ellps=sphere +datum=WGS84 +no_defs",
                                                        NULL);

      if (m_wgs84CoordinateSystem_p == NULL)
      {
        throw EAGGRException("Failed to create WGS84 coordinate system.");
      }

      m_sphereCoordinateSystem_p = proj_create_crs_to_crs(PJ_DEFAULT_CTX,
                                                        "+proj=longlat +ellps=sphere +datum=WGS84 +no_defs",
                                                        "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs",
                                                        NULL);
      if (m_sphereCoordinateSystem_p == NULL)
      {
        throw EAGGRException("Failed to create spherical coordinate system.");
      }
    }

    CoordinateConverter::~CoordinateConverter()
    {
      if (m_wgs84CoordinateSystem_p != NULL)
      { 
          proj_destroy(m_wgs84CoordinateSystem_p);
      }

      if (m_sphereCoordinateSystem_p != NULL)
      { 
          proj_destroy(m_sphereCoordinateSystem_p);
      }
    }

    EAGGR::LatLong::SphericalAccuracyPoint CoordinateConverter::ConvertWGS84ToSphere(
        const EAGGR::LatLong::Wgs84AccuracyPoint a_wgs84Point) const
    {
      EAGGR::Utilities::Maths::Radians latitudeRadians = a_wgs84Point.GetLatitudeInRadians();
      EAGGR::Utilities::Maths::Radians longitudeRadians = a_wgs84Point.GetLongitudeInRadians();

      EAGGR::Utilities::Maths::Radians convertedLatitude = latitudeRadians;
      EAGGR::Utilities::Maths::Radians convertedLongitude = longitudeRadians;

      PJ_COORD c;
      // Initially assume a height of zero and perform the conversion
      c.lpzt.z     = 0.0;
      c.lpzt.t       = HUGE_VAL;
      c.xy.x       = convertedLongitude;
      c.xy.y       = convertedLatitude;

      int returnCode = proj_trans_array(m_wgs84CoordinateSystem_p,
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
      convertedLatitude = latitudeRadians;
      convertedLongitude = longitudeRadians;
      
      c.lpzt.z = -c.lpzt.z;
      c.lpzt.t = HUGE_VAL;
      c.xy.x   = longitudeRadians;
      c.xy.y   = latitudeRadians;

      returnCode = proj_trans_array(m_wgs84CoordinateSystem_p, PJ_FWD, m_NUM_POINTS_TO_CONVERT, &c);

      if (returnCode != 0)
      {
        const char* error = proj_errno_string(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      return EAGGR::LatLong::SphericalAccuracyPoint(
          RADIANS_IN_DEG(convertedLatitude),
          RADIANS_IN_DEG(convertedLongitude),
          EAGGR::LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(
              a_wgs84Point.GetAccuracy()));
    }

    EAGGR::LatLong::Wgs84AccuracyPoint CoordinateConverter::ConvertSphereToWGS84(
        const EAGGR::LatLong::SphericalAccuracyPoint a_spherePoint) const
    {
      double latitudeRadians = a_spherePoint.GetLatitudeInRadians();
      double longitudeRadians = a_spherePoint.GetLongitudeInRadians();

      double z = 0.0;

      PJ_COORD c;
      c.lpzt.z = 0.0;
      c.lpzt.t = HUGE_VAL;
      c.xy.x   = longitudeRadians;
      c.xy.y   = latitudeRadians;

      int returnCode = proj_trans_array(m_sphereCoordinateSystem_p, PJ_FWD, m_NUM_POINTS_TO_CONVERT, &c);

      if (returnCode != 0)
      {
        const char* error = proj_errno_string(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      return EAGGR::LatLong::Wgs84AccuracyPoint(
          RADIANS_IN_DEG(latitudeRadians),
          RADIANS_IN_DEG(longitudeRadians),
          EAGGR::LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(
              a_spherePoint.GetAccuracy()));
    }
  }
}
