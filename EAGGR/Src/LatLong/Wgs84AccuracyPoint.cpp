//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84AccuracyPoint.cpp
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

#include <cmath>
#include <algorithm>
#include <limits>

#include "Wgs84AccuracyPoint.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace LatLong
  {
    Wgs84AccuracyPoint::Wgs84AccuracyPoint(
        const Degrees a_latitude,
        const Degrees a_longitude,
        const Degrees a_accuracy)
        : Point(a_latitude, a_longitude), m_accuracy(a_accuracy)
    {
      if (a_accuracy < 0.0)
      {
        throw EAGGR::EAGGRException("Accuracy cannot be negative.");
      }
    }

    double Wgs84AccuracyPoint::GetAccuracy() const
    {
      return (m_accuracy);
    }

    bool Wgs84AccuracyPoint::operator==(const Wgs84AccuracyPoint & a_otherPoint) const
    {
      return (this->GetLatitude() == a_otherPoint.GetLatitude())
          && (this->GetLongitude() == a_otherPoint.GetLongitude())
          && (this->GetAccuracy() == a_otherPoint.GetAccuracy());
    }

    bool Wgs84AccuracyPoint::operator!=(const Wgs84AccuracyPoint & a_otherPoint) const
    {
      return !(*this == a_otherPoint);
    }
  }
}
