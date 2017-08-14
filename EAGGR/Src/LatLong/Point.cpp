//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Point.cpp
/// 
/// Implements the EAGGR::LatLong::Point class.
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

#include <cmath>

#include "Point.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace LatLong
  {
    Point::Point(Degrees a_latitude, Degrees a_longitude)
    {
      static const double LAT_LONG_TOLERANCE = 1E-12;

      double minLat = m_MIN_LATITUDE;
      double maxLat = m_MAX_LATITUDE;
      double minLong = m_MIN_LONGITUDE;
      double maxLong = m_MAX_LONGITUDE;

      if (a_latitude > maxLat + LAT_LONG_TOLERANCE)
      {
        throw EAGGR::EAGGRException(
            "Latitude is greater than maximum allowed for a lat/long point.");
      }

      if (a_latitude < minLat - LAT_LONG_TOLERANCE)
      {
        throw EAGGR::EAGGRException("Latitude is less than minimum allowed for a lat/long point.");
      }

      if (a_longitude > maxLong + LAT_LONG_TOLERANCE)
      {
        throw EAGGR::EAGGRException(
            "Longitude is greater than maximum allowed for a lat/long point.");
      }

      if (a_longitude < minLong - LAT_LONG_TOLERANCE)
      {
        throw EAGGR::EAGGRException("Longitude is less than maximum allowed for a lat/long point.");
      }

      // Cap the latitude and longitude within the limits (if they are outside the limit within the tolerance)
      m_latitude = std::min(std::max(a_latitude, minLat), maxLat);
      m_longitude = std::min(std::max(a_longitude, minLong), maxLong);
    }

    Degrees Point::GetLatitude() const
    {
      return m_latitude;
    }

    Degrees Point::GetLongitude() const
    {
      return m_longitude;
    }

    Radians Point::GetLatitudeInRadians() const
    {
      return DEGREES_IN_RAD(m_latitude);
    }

    Radians Point::GetLongitudeInRadians() const
    {
      return DEGREES_IN_RAD(m_longitude);
    }

    bool Point::IsCloseTo(
        const Point & a_point,
        const Degrees a_latitudeTolerance,
        const Degrees a_longitudeTolerance) const
    {
      if (std::abs(m_latitude - a_point.GetLatitude()) > a_latitudeTolerance)
      {
        return false;
      }
      if (std::abs(
          std::remainder(m_longitude - a_point.GetLongitude(), m_MAX_LONGITUDE - m_MIN_LONGITUDE))
          > a_longitudeTolerance)
      {
        return false;
      }

      return true;
    }

    Utilities::Maths::Degrees Point::WrapLongitude(const Utilities::Maths::Degrees a_longitude)
    {
      Utilities::Maths::Degrees longitude = a_longitude;

      while (longitude > m_MAX_LONGITUDE)
      {
        longitude -= (m_MAX_LONGITUDE - m_MIN_LONGITUDE);
      }

      while (longitude < m_MIN_LONGITUDE)
      {
        longitude += (m_MAX_LONGITUDE - m_MIN_LONGITUDE);
      }

      return longitude;
    }
  }
}
