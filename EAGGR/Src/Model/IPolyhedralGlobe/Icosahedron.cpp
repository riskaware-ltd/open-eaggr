//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Polyhedral Globe
//
//------------------------------------------------------
/// @file Icosahedron.cpp
/// 
/// Implements the EAGGR::Model::PolyhedralGlobe::Icosahedron class.
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

#include <string>
#include <sstream>

#include "Icosahedron.hpp"

#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace PolyhedralGlobe
    {
      FaceIndex Icosahedron::GetNoOfFaces() const
      {
        return (20U);
      }

      Utilities::Maths::Radians Icosahedron::Get_g() const
      {
        return (DEGREES_IN_RAD(37.37736814));
      }

      Utilities::Maths::Radians Icosahedron::Get_G() const
      {
        return (DEGREES_IN_RAD(36.0));
      }

      Utilities::Maths::Radians Icosahedron::GetTheta() const
      {
        return (DEGREES_IN_RAD(30.0));
      }

      double Icosahedron::GetRPrimeRelativeToR() const
      {
        // Equation 1 (in terms of R squared)
        // AGT = (G - Theta) * R

        // Equation 3 (in terms of RPrime squared)
        // AMT = (0.5 * tan2(g) * sin(Theta) * cos(Theta)) * RPrime

        // Equating the two equations gives RPrime (in terms of R)
        // RPrime = sqrt(AGT / AMT) * R

        return (0.91038328153090290025);
      }

      double Icosahedron::GetEdgeLengthRelativeToRPrime() const
      {
        // Distance to from the globe's centre to the face centre is the same for
        // all faces of an icosahedron so the returned value is always the same

        // Edge Length = Distance to Face Centre * 12 / ( sqrt(3) * (3 + sqrt(5)) )
        return (1.323169076499215);
      }

      Utilities::Maths::Radians Icosahedron::GetOrientationOfFace(const FaceIndex a_faceIndex) const
      {
        if (a_faceIndex >= GetNoOfFaces())
        {
          std::stringstream stream;
          stream << "Unknown face index (" << a_faceIndex << ")";
          throw EAGGRException(stream.str());
        }

        if (a_faceIndex <= 4U || (a_faceIndex >= 10U && a_faceIndex <= 14U))
        {
          return (DEGREES_IN_RAD(0.0));
        }
        else
        {
          return (DEGREES_IN_RAD(60.0));
        }
      }

      LatLong::Point Icosahedron::GetFaceCentre(const FaceIndex a_faceIndex) const
      {
        static const Utilities::Maths::Degrees E = 52.62263186;
        static const Utilities::Maths::Degrees F = 10.81231696;

        switch (a_faceIndex)
        {
          case 0:
            return (LatLong::Point(E, -144.0));
          case 1:
            return (LatLong::Point(E, -72.0));
          case 2:
            return (LatLong::Point(E, 0.0));
          case 3:
            return (LatLong::Point(E, 72.0));
          case 4:
            return (LatLong::Point(E, 144.0));
          case 5:
            return (LatLong::Point(F, -144.0));
          case 6:
            return (LatLong::Point(F, -72.0));
          case 7:
            return (LatLong::Point(F, 0.0));
          case 8:
            return (LatLong::Point(F, 72.0));
          case 9:
            return (LatLong::Point(F, 144.0));
          case 10:
            return (LatLong::Point(-F, -108.0));
          case 11:
            return (LatLong::Point(-F, -36.0));
          case 12:
            return (LatLong::Point(-F, 36.0));
          case 13:
            return (LatLong::Point(-F, 108.0));
          case 14:
            return (LatLong::Point(-F, 180.0));
          case 15:
            return (LatLong::Point(-E, -108.0));
          case 16:
            return (LatLong::Point(-E, -36.0));
          case 17:
            return (LatLong::Point(-E, 36.0));
          case 18:
            return (LatLong::Point(-E, 108.0));
          case 19:
            return (LatLong::Point(-E, 180.0));
          default:
            std::stringstream stream;
            stream << "Unknown face index (" << a_faceIndex << ")";
            throw EAGGRException(stream.str());
        }
      }
    }
  }
}
