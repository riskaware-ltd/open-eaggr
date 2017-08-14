//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file IPolyhedralGlobe.hpp
/// 
/// Implements the EAGGR::Model::PolyhedralGlobe::IPolyhedralGlobe class.
///
/// @note Only grids that are based on equilateral triangle faces are currently supported by the DGGS library.
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

#include "Src/Utilities/Maths.hpp"
#include "Src/Model/FaceTypes.hpp"
#include "Src/LatLong/Point.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace PolyhedralGlobe
    {
      /// Interface for a polyhedral globe.
      class IPolyhedralGlobe
      {
        public:
          /// Destructor
          virtual ~IPolyhedralGlobe()
          {
          }
          ;

          /// @return The number of faces on the polyhedral globe.
          virtual FaceIndex GetNoOfFaces() const = 0;

          /// @return The spherical distance from centre of the polygon face to any of its vertices on the globe.
          virtual Utilities::Maths::Radians Get_g() const = 0;

          /// @return The spherical angle between radius vector to the face centre and adjacent edge of spherical
          /// polygon on the globe.
          virtual Utilities::Maths::Radians Get_G() const = 0;

          /// @return The plane angle between radius vector to the face centre and adjacent edge of plane polygon.
          virtual Utilities::Maths::Radians GetTheta() const = 0;

          /// @return The distance from the centre of the polyhedron to the centre of the specified face.
          /// Distance is given relative to the radius of a sphere of the same surface area.
          virtual double GetRPrimeRelativeToR() const = 0;

          /// @return The ratio of the polyhedron's edge length relative to the distance between the
          /// centre of the polygon to the centre of the specified face.
          virtual double GetEdgeLengthRelativeToRPrime() const = 0;

          /// @param a_faceIndex The index of the face to get the orientation for.
          /// @return The angle of the face relative to the top of the polyhedron (vertex 0).
          /// @throws DGGSException if the specified face does not exist.
          virtual Utilities::Maths::Radians GetOrientationOfFace(
              const FaceIndex a_faceIndex) const = 0;

          /// @param a_faceIndex The index of the face to get the face centre for.
          /// @return The latitude and longitude of the centre of the face on the polyhedron.
          /// @throws DGGSException if the specified face does not exist.
          virtual LatLong::Point GetFaceCentre(const FaceIndex a_faceIndex) const = 0;
      };
    }
  }
}
