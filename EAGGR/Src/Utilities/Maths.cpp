//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Utilities
//
//------------------------------------------------------
/// @file Maths.cpp
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

#include <cmath>

#include "Maths.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Utilities
  {
    namespace Maths
    {
      double Cot(const Radians a_angle)
      {
        return (1 / tan(a_angle));
      }

      double Squared(const double a_input)
      {
        return (a_input * a_input);
      }

      double GetRadiusFromCircleArea(const double a_area)
      {
        if (a_area < 0.0)
        {
          throw EAGGR::EAGGRException("Circle area cannot be negative.");
        }

        return (sqrt(a_area / PI));
      }

      double OneMinusCos(const Radians a_angle)
      {
        double oneMinusCosAngle = 1.0 - cos(a_angle);

        // If the angle is too small then use the small angle approximation
        // 1.0 - cos(x) ~= 1.0 - (1.0 - (x^2 / 2.0)) = x^2 / 2.0
        if (oneMinusCosAngle == 0.0)
        {
          oneMinusCosAngle = (Squared(a_angle) / 2.0);
        }

        return oneMinusCosAngle;
      }
    }
  }
}
