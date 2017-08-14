//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file CartesianPoint.cpp
/// 
/// Implements the EAGGR::Model::CartesianPoint class.
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

#include "CartesianPoint.hpp"
#include "Src/Utilities/Maths.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace Model
  {
    CartesianPoint::CartesianPoint(const double a_x, const double a_y)
        : m_x(a_x), m_y(a_y)
    {

    }

    double CartesianPoint::GetX() const
    {
      return m_x;
    }

    double CartesianPoint::GetY() const
    {
      return m_y;
    }

    void CartesianPoint::SetX(const double a_x)
    {
      m_x = a_x;
    }

    void CartesianPoint::SetY(const double a_y)
    {
      m_y = a_y;
    }

    double CartesianPoint::GetDistanceToPoint(const CartesianPoint & a_point) const
    {
      const double xDistance = GetX() - a_point.GetX();
      const double yDistance = GetY() - a_point.GetY();

      return (sqrt(Squared(xDistance) + Squared(yDistance)));
    }
  }
}
