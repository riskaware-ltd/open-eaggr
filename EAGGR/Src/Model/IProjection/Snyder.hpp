//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Projection
//
//------------------------------------------------------
/// @file Snyder.hpp
/// 
/// Implements the EAGGR::Model::Projection::Snyder class.
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
#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/IProjection.hpp"
#include "Src/Model/IPolyhedralGlobe.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Projection
    {
      /// Implements the Snyder Equal Area projection on to a polyhedral globe.
      /// @note Implementation is based on "An Equal-Area Projection for Polyhedral Globes",
      ///       John P Snyder, Cartographica Vol. 29 No 1, Spring 1992, pp. 10-21.
      class Snyder: virtual public IProjection
      {
        public:
          /// Constructor.
          /// @param a_pGlobe The polyhedral globe to project onto.
          Snyder(const PolyhedralGlobe::IPolyhedralGlobe * const a_pGlobe);

          /// Destructor
          virtual ~Snyder()
          {
          }
          ;

          /// @param a_point The lat/long point to project
          /// @return The location of a point on a polyhedron face from
          /// projecting a lat/long point in spherical coordinates
          virtual FaceCoordinate GetFaceCoordinate(
              const LatLong::SphericalAccuracyPoint a_point) const;

          /// @param a_coordinate The coordinate to project
          /// @return The lat/long point in spherical coordinates from
          /// projecting a point on a polyhedron face to the Earth
          virtual LatLong::SphericalAccuracyPoint GetLatLongPoint(
              const FaceCoordinate a_coordinate) const;

        private:
          /// Pointer to the polyhedral globe used for the projection.
          const PolyhedralGlobe::IPolyhedralGlobe * const m_pGlobe;

          /// Adjusts an angle so it is between 0 and the specified angle.
          /// @param a_theta The plane angle in radians between radius vector to centre and adjacent edge of plane polygon.
          /// @param a_angle The value of the angle to be adjusted in radians.
          ///        This variable gets modified to lie in the range 0 to a_theta
          /// @return The number of radians that the angle was adjusted.
          Utilities::Maths::Radians AdjustAz(
              const Utilities::Maths::Radians a_theta,
              Utilities::Maths::Radians & a_angle) const;

          /// @return The length of an edge on the polyhedron in terms of the radius of the earth.
          double GetEdgeLengthRelativeToR() const;

          /// @param a_accuracyAngle The angle defining the point accuracy
          /// @return The area defined by the accuracy of the lat / long coordinates relative to the
          /// area of a face of the polyhedral globe.
          double GetAccuracyArea(const Utilities::Maths::Degrees a_accuracyAngle) const;

          /// @param a_accuracyArea The area defining the point accuracy
          /// @return The accuracy area of a face coordinate as an angle which defines a spherical cap
          /// of same area.
          Utilities::Maths::Degrees GetAccuracyAngle(const double a_accuracyArea) const;
      };
    }
  }
}
