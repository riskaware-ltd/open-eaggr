//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84AccuracyPoint.hpp
/// 
/// Implements the EAGGR::LatLong::Wgs84AccuracyPoint class.
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

#include "Src/LatLong/Point.hpp"
#include "Src/LatLong/IShape.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    /// Describes a point on the surface of the earth using WGS84 latitude and longitude coordinates.
    class Wgs84AccuracyPoint: public Point, public IShape
    {
      public:
        /// Constructor
        /// @param a_latitude The latitude in degrees
        /// @param a_longitude The longitude in degrees
        /// @param a_accuracy The angle defining the accuracy of the point in metres squared
        /// @throws DGGSException if supplied latitude or longitude are out of range or accuracy is negative
        Wgs84AccuracyPoint(
            const Utilities::Maths::Degrees a_latitude,
            const Utilities::Maths::Degrees a_longitude,
            const Utilities::Maths::Degrees a_accuracy);

        virtual ~Wgs84AccuracyPoint();

        /// @return The accuracy of the point as an angle in metres squared
        double GetAccuracy() const;

        /// Equal to operator, used for comparing two Wgs84AccuracyPoint objects.
        /// @param a_otherPoint Wgs84AccuracyPoint object to compare against.
        /// @return True if the points have the same latitude, longitude and accuracy; otherwise false.
        bool operator==(const Wgs84AccuracyPoint & a_otherPoint) const;

        /// @param a_otherPoint Wgs84AccuracyPoint object to compare against.
        /// @return Inverse of the == operator.
        bool operator!=(const Wgs84AccuracyPoint & a_otherPoint) const;

      private:
        double m_accuracy;
    };
  }
}
