//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file CartesianPoint.hpp
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

#pragma once

namespace EAGGR
{
  namespace Model
  {
    /// Represents an x, y point in a plane.
    class CartesianPoint
    {
      public:
        /// Constructor.
        /// @param a_x The x coordinate of the point.
        /// @param a_y The y coordinate of the point.
        CartesianPoint(const double a_x, const double a_y);

        /// @return The x coordinate of the point.
        double GetX() const;

        /// @return The y coordinate of the point.
        double GetY() const;

        /// Sets the x coordinate of the point.
        /// @param a_x The value of the x coordinate.
        void SetX(const double a_x);

        /// Sets the y coordinate of the point.
        /// @param a_y The value of the y coordinate.
        void SetY(const double a_y);

        /// Uses the Pythagorean theorem to calculate the distance to the
        /// specified point.
        /// @param a_point The point to get the distance to.
        double GetDistanceToPoint(const CartesianPoint & a_point) const;

      private:
        double m_x;
        double m_y;
    };

  }
}
