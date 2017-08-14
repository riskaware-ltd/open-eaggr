//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file SphericalAccuracyPoint.cpp
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

#include <cmath>
#include <algorithm>
#include <limits>

#include "SphericalAccuracyPoint.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace LatLong
  {
    SphericalAccuracyPoint::SphericalAccuracyPoint(
        const Degrees a_latitude,
        const Degrees a_longitude,
        const Degrees a_accuracy)
        : Point(a_latitude, a_longitude), m_accuracy(a_accuracy)
    {
      if (a_accuracy < 0.0)
      {
        throw EAGGR::EAGGRException("Accuracy angle cannot be negative.");
      }

      if (a_accuracy > 360.0)
      {
        throw EAGGR::EAGGRException("Accuracy angle must be less than 360 degrees.");
      }
    }

    Utilities::Maths::Degrees SphericalAccuracyPoint::SquareMetresToAngleAccuracy(
        const double a_squareMetres)
    {
      if (a_squareMetres < 0.0)
      {
        throw EAGGR::EAGGRException("Cannot calculate an angle for negative area.");
      }

      const double areaToSphereRatio = a_squareMetres / (2.0 * PI * Squared(m_EARTH_RADIUS));

      Radians angleAccuracy = acos(1.0 - areaToSphereRatio);

      // Due to numerical precision the angleAccuracy can become zero (for small areas)
      // In this instance use the small angle approximation formula for cos(x) = 1 - (x^2 / 2)
      // https://en.wikipedia.org/wiki/Small-angle_approximation
      // From above cos(angleAccuracy) ~= 1.0 - (angleAccuracy^2) / 2.0 = 1.0 - areaToSphereRatio
      if (angleAccuracy == 0.0)
      {
        angleAccuracy = sqrt(2.0 * areaToSphereRatio);
      }

      return RADIANS_IN_DEG(angleAccuracy);
    }

    double SphericalAccuracyPoint::AngleAccuracyToSquareMetres(
        const Utilities::Maths::Degrees a_angleAccuracy)
    {
      if (a_angleAccuracy < 0.0)
      {
        throw EAGGR::EAGGRException("Cannot calculate an area for negative angle.");
      }

      double squareMetres = (Utilities::Maths::OneMinusCos(DEGREES_IN_RAD(a_angleAccuracy)))
          * (2.0 * PI * Squared(m_EARTH_RADIUS));

      return squareMetres;
    }

    Degrees SphericalAccuracyPoint::GetAccuracy() const
    {
      return (m_accuracy);
    }

    double SphericalAccuracyPoint::GetDistanceToPoint(const SphericalAccuracyPoint & a_point) const
    {
      // Using the 'Haversine' formula:
      // http://www.movable-type.co.uk/scripts/latlong.html

      const Radians phi1 = this->GetLatitudeInRadians();
      const Radians lambda1 = this->GetLongitudeInRadians();

      const Radians phi2 = a_point.GetLatitudeInRadians();
      const Radians lambda2 = a_point.GetLongitudeInRadians();

      const double a = Utilities::Maths::Squared(sin((phi2 - phi1) / 2.0))
          + (cos(phi1) * cos(phi2) * Utilities::Maths::Squared(sin((lambda2 - lambda1) / 2.0)));

      const double c = 2 * atan2(sqrt(a), sqrt(1 - a));

      const double d = m_EARTH_RADIUS * c;

      return d;
    }
  }
}
