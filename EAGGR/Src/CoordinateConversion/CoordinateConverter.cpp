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

#include "CoordinateConverter.hpp"

#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace CoordinateConversion
  {
    CoordinateConverter::CoordinateConverter()
    {
      m_wgs84CoordinateSystem = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");

      if (m_wgs84CoordinateSystem == NULL)
      {
        throw EAGGRException("Failed to create WGS84 coordinate system.");
      }

      m_sphereCoordinateSystem = pj_init_plus("+proj=longlat +ellps=sphere +datum=WGS84 +no_defs");

      if (m_sphereCoordinateSystem == NULL)
      {
        throw EAGGRException("Failed to create spherical coordinate system.");
      }
    }

    CoordinateConverter::~CoordinateConverter()
    {
      if (m_wgs84CoordinateSystem != NULL)
      {
        pj_free (m_wgs84CoordinateSystem);
      }

      if (m_sphereCoordinateSystem != NULL)
      {
        pj_free (m_sphereCoordinateSystem);
      }
    }

    EAGGR::LatLong::SphericalAccuracyPoint CoordinateConverter::ConvertWGS84ToSphere(
        const EAGGR::LatLong::Wgs84AccuracyPoint a_wgs84Point) const
    {
      EAGGR::Utilities::Maths::Radians latitudeRadians = a_wgs84Point.GetLatitudeInRadians();
      EAGGR::Utilities::Maths::Radians longitudeRadians = a_wgs84Point.GetLongitudeInRadians();

      EAGGR::Utilities::Maths::Radians convertedLatitude = latitudeRadians;
      EAGGR::Utilities::Maths::Radians convertedLongitude = longitudeRadians;

      // Initially assume a height of zero and perform the conversion
      double z = 0.0;

      int returnCode = pj_transform(
          m_wgs84CoordinateSystem,
          m_sphereCoordinateSystem,
          m_NUM_POINTS_TO_CONVERT,
          m_POINT_OFFSET,
          &convertedLongitude,
          &convertedLatitude,
          &z);

      if (returnCode != 0)
      {
        char* error = pj_strerrno(returnCode);
        std::stringstream stream;
        stream << "Coordinate transformation error: " << error;
        throw EAGGRException(stream.str());
      }

      // First conversion provides a height offset with an offset in the latitude/longitude values
      // Need to perform the conversion again on the original lat/long using the negated height offset
      // This provides the correct lat/long to a good approximation.
      convertedLatitude = latitudeRadians;
      convertedLongitude = longitudeRadians;
      z = -z;

      returnCode = pj_transform(
          m_wgs84CoordinateSystem,
          m_sphereCoordinateSystem,
          m_NUM_POINTS_TO_CONVERT,
          m_POINT_OFFSET,
          &convertedLongitude,
          &convertedLatitude,
          &z);

      if (returnCode != 0)
      {
        char* error = pj_strerrno(returnCode);
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

      int returnCode = pj_transform(
          m_sphereCoordinateSystem,
          m_wgs84CoordinateSystem,
          m_NUM_POINTS_TO_CONVERT,
          m_POINT_OFFSET,
          &longitudeRadians,
          &latitudeRadians,
          &z);

      if (returnCode != 0)
      {
        char* error = pj_strerrno(returnCode);
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
