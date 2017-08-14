//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file SphericalAccuracyPoint.hpp
/// 
/// Implements the EAGGR::LatLong::SphericalAccuracyPoint class.
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

namespace EAGGR
{
  namespace LatLong
  {
    /// Describes a point on the surface of the earth using spherical earth latitude and longitude coordinates.
    class SphericalAccuracyPoint: public Point
    {
      public:
        /// Converts an area in metres squared to an angle that defines a spherical cap of the same
        /// area (accuracy angle). Can be used in conjunction with the constructor to create a point
        /// whose accuracy is specified as an area in metres squared.
        /// @param a_squareMetres The accuracy in metres squared.
        /// @return The angle defining a spherical cap of the specified area.
        /// @throws DGGSException if supplied accuracy is negative
        static Utilities::Maths::Degrees SquareMetresToAngleAccuracy(const double a_squareMetres);

        /// Converts an angle that defines a spherical cap to an area in metres squared.
        /// @param a_angleAccuracy The angle defining the accuracy as a spherical cap.
        /// @return The accuracy in metres squared.
        /// @throws DGGSException if supplied accuracy is negative
        static double AngleAccuracyToSquareMetres(const Utilities::Maths::Degrees a_angleAccuracy);

        /// Constructor
        /// @param a_latitude The latitude in degrees
        /// @param a_longitude The longitude in degrees
        /// @param a_accuracy The angle defining the accuracy of the point in degrees
        /// @throws DGGSException if supplied latitude or longitude are out of range or accuracy is not in the range 0 to 360 degrees
        SphericalAccuracyPoint(
            const Utilities::Maths::Degrees a_latitude,
            const Utilities::Maths::Degrees a_longitude,
            const Utilities::Maths::Degrees a_accuracy);

        /// @return The accuracy of the point as an angle in degrees that defines a spherical cap
        /// around the point
        Utilities::Maths::Degrees GetAccuracy() const;

        /// Uses the 'Haversine' formula to determine the spherical distance in
        /// metres to the specified point.
        /// @param a_point The point to get the distance to.
        /// @return The distance in metres from this point to the specified point.
        double GetDistanceToPoint(const SphericalAccuracyPoint & a_point) const;

      private:
        Utilities::Maths::Degrees m_accuracy;
    };
  }
}
