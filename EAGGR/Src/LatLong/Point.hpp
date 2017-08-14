//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Point.hpp
/// 
/// Implements the EAGGR::Model::Point class.
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

#include "Src/Utilities/Maths.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    /// Defines a point on the polyhedral globe in latitude and longitude.
    class Point
    {
      public:
        /// The minimum valid latitude value in degrees
        static constexpr Utilities::Maths::Degrees m_MIN_LATITUDE = -90.0;

        /// The maximum valid latitude value in degrees
        static constexpr Utilities::Maths::Degrees m_MAX_LATITUDE = 90.0;

        /// The minimum valid longitude value in degrees
        static constexpr Utilities::Maths::Degrees m_MIN_LONGITUDE = -180.0;

        /// The maximum valid longitude value in degrees
        static constexpr Utilities::Maths::Degrees m_MAX_LONGITUDE = 180.0;

        /// Radius of earth in metres (WGS84 authalic sphere radius)
        static constexpr double m_EARTH_RADIUS = 6371007.180918475;

        /// Wraps the supplied longitude value in degrees to be within the range [MIN_LONGITUDE, MAX_LONGITUDE]
        static Utilities::Maths::Degrees WrapLongitude(const Utilities::Maths::Degrees a_longitude);

        /// Constructor.
        /// @param a_latitude The latitude in degrees.
        /// @param a_longitude The longitude in degrees.
        /// @throws DGGSException if supplied latitude or longitude are out of range
        Point(Utilities::Maths::Degrees a_latitude, Utilities::Maths::Degrees a_longitude);

        /// @return The latitude in degrees
        Utilities::Maths::Degrees GetLatitude() const;

        /// @return The longitude in degrees
        Utilities::Maths::Degrees GetLongitude() const;

        /// @return The latitude in radians
        Utilities::Maths::Radians GetLatitudeInRadians() const;

        /// @return The longitude in radians
        Utilities::Maths::Radians GetLongitudeInRadians() const;

        /// Method for comparing two points that takes into account that the world is
        /// round so -180 degrees longitude is the same as 180 degrees.
        /// @param a_point Point to compare.
        /// @param a_latitudeTolerance Angle in the latitude direction inside which
        ///        the point is considered as being close.
        /// @param a_longitudeTolerance Angle in the longitude direction inside which
        ///        the point is considered as being close.
        /// @return True if the point is within the specified tolerances of the given
        ///         point, otherwise false.
        bool IsCloseTo(
            const Point & a_point,
            const Utilities::Maths::Degrees a_latitudeTolerance,
            const Utilities::Maths::Degrees a_longitudeTolerance) const;

      private:
        Utilities::Maths::Degrees m_latitude;
        Utilities::Maths::Degrees m_longitude;
    };
  }
}
