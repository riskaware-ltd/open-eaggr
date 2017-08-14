//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84Linestring.cpp
/// 
/// Implements the EAGGR::LatLong::Wgs84Linestring class.
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

#include "Src/LatLong/Wgs84Linestring.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    Wgs84Linestring::Wgs84Linestring()
    {
      m_points.empty();
    }

    Wgs84Linestring::~Wgs84Linestring()
    {
      for (std::vector<Wgs84AccuracyPoint *>::iterator it = m_points.begin(); it != m_points.end();
          ++it)
      {
        delete *it;
      }
    }

    void Wgs84Linestring::AddAccuracyPoint(
        const Utilities::Maths::Degrees a_latitude,
        const Utilities::Maths::Degrees a_longitude,
        const double a_accuracy)
    {
      m_points.push_back(new Wgs84AccuracyPoint(a_latitude, a_longitude, a_accuracy));
    }

    unsigned short Wgs84Linestring::GetNumberOfPoints() const
    {
      return m_points.size();
    }

    const Wgs84AccuracyPoint * Wgs84Linestring::GetAccuracyPoint(
        const unsigned short a_pointIndex) const
    {
      CheckPointIsValid(a_pointIndex);

      return m_points[a_pointIndex];
    }

    bool Wgs84Linestring::operator==(const Wgs84Linestring & a_otherLinestring) const
    {
      if (GetNumberOfPoints() != a_otherLinestring.GetNumberOfPoints())
      {
        return false;
      }

      for (unsigned short pointIndex = 0U; pointIndex < GetNumberOfPoints(); pointIndex++)
      {
        if (*(GetAccuracyPoint(pointIndex)) != *(a_otherLinestring.GetAccuracyPoint(pointIndex)))
        {
          return false;
        }
      }

      return true;
    }

    bool Wgs84Linestring::operator!=(const Wgs84Linestring & a_otherLinestring) const
    {
      return !(*this == a_otherLinestring);
    }

    void Wgs84Linestring::CheckPointIsValid(const unsigned short a_pointIndex) const
    {
      if (a_pointIndex >= m_points.size())
      {
        std::stringstream stream;
        stream << "Linestring does not contain a point at index " << a_pointIndex;
        throw EAGGRException(stream.str());
      }
    }
  }
}
