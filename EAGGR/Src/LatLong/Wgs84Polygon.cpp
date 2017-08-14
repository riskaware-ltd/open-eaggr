//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84Polygon.cpp
/// 
/// Implements the EAGGR::LatLong::Wgs84Polygon class.
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

#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    Wgs84Polygon::Wgs84Polygon()
    {
      m_innerRings.empty();
    }

    Wgs84Polygon::~Wgs84Polygon()
    {
      for (std::vector<Wgs84Linestring *>::iterator it = m_innerRings.begin();
          it != m_innerRings.end(); ++it)
      {
        delete *it;
      }
    }

    void Wgs84Polygon::AddAccuracyPointToOuterRing(
        const Utilities::Maths::Degrees a_latitude,
        const Utilities::Maths::Degrees a_longitude,
        const double a_accuracy)
    {
      m_outerRing.AddAccuracyPoint(a_latitude, a_longitude, a_accuracy);
    }

    const Wgs84Linestring * Wgs84Polygon::GetOuterRing() const
    {
      return &m_outerRing;
    }

    void Wgs84Polygon::CreateInnerRing()
    {
      m_innerRings.push_back(new Wgs84Linestring());
    }

    void Wgs84Polygon::AddAccuracyPointToInnerRing(
        const unsigned short a_ringIndex,
        const Utilities::Maths::Degrees a_latitude,
        const Utilities::Maths::Degrees a_longitude,
        const double a_accuracy)
    {
      CheckInnerRingIsValid(a_ringIndex);

      m_innerRings[a_ringIndex]->AddAccuracyPoint(a_latitude, a_longitude, a_accuracy);
    }

    unsigned short Wgs84Polygon::GetNumberOfInnerRings() const
    {
      return m_innerRings.size();
    }

    const Wgs84Linestring * Wgs84Polygon::GetInnerRing(const unsigned short a_ringIndex) const
    {
      CheckInnerRingIsValid(a_ringIndex);

      return m_innerRings[a_ringIndex];
    }

    bool Wgs84Polygon::operator==(const Wgs84Polygon & a_otherPolygon) const
    {
      if (*(GetOuterRing()) != *(a_otherPolygon.GetOuterRing()))
      {
        return false;
      }

      if (GetNumberOfInnerRings() != a_otherPolygon.GetNumberOfInnerRings())
      {
        return false;
      }

      for (unsigned short ringIndex = 0U; ringIndex < GetNumberOfInnerRings(); ringIndex++)
      {
        if (*(GetInnerRing(ringIndex)) != *(a_otherPolygon.GetInnerRing(ringIndex)))
        {
          return false;
        }
      }

      return true;
    }

    bool Wgs84Polygon::operator!=(const Wgs84Polygon & a_otherPolygon) const
    {
      return !(*this == a_otherPolygon);
    }

    void Wgs84Polygon::CheckInnerRingIsValid(const unsigned short a_ringIndex) const
    {
      if (a_ringIndex >= m_innerRings.size())
      {
        std::stringstream stream;
        stream << "Specified inner ring, '" << a_ringIndex << "', does not exist";
        throw EAGGRException(stream.str());
      }
    }
  }
}
