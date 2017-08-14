//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Utilities
//
//------------------------------------------------------
/// @file Maths.hpp
/// 
/// Provides mathematical functions and type definitions
/// that are not included in the standard cmath library.
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
  namespace Utilities
  {
    namespace Maths
    {
      typedef double Degrees;
      typedef double Radians;

      typedef double SquareMetres;

      /// @param a_angle The angle to process.
      /// @return The cotangent of the specified angle.
      double Cot(const Radians a_angle);

      /// @param a_input The value to process.
      /// @return The square of the specified value.
      double Squared(double a_input);

      /// @param a_area The area of the circle.
      /// @return The radius of the circle with the specified area.
      /// @throws DGGSException if the supplied area is negative
      double GetRadiusFromCircleArea(const double a_area);

      /// Implementation of the 1.0 - cos(a_angle) function which uses the small-angle approximation for cos
      /// if the angle is small (i.e. cos(a_angle) == 1.0 within numerical precision).
      ///
      /// Small angle approximation formula for cos(x) = 1 - (x^2 / 2)
      /// - https://en.wikipedia.org/wiki/Small-angle_approximation
      /// @param a_angle The angle to process.
      /// @return The value of 1.0 - cos(a_angle) of the specified angle.
      double OneMinusCos(const Radians a_angle);
    }
  }
}
