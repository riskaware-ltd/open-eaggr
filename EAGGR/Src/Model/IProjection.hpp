//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file IProjection.hpp
///
/// Implements the EAGGR::Model::Projection::IProjection class.
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

#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/Model/FaceCoordinate.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Projection
    {
      /// Interface for a projection from spherical latitude/longitude coordinates on to a polyhedral globe.
      class IProjection
      {
        public:
          /// Destructor.
          virtual ~IProjection()
          {
          }
          ;

          /// Converts a lat/long point on the earth to a coordinate on the face of a polyhedron.
          /// @return The face coordinate obtained by projecting the supplied point.
          virtual FaceCoordinate GetFaceCoordinate(
              const LatLong::SphericalAccuracyPoint a_point) const = 0;

          /// Converts a coordinate on the face of a polyhedron to a lat/long point on the earth.
          /// @return The point obtained by projecting the supplied coordinate.
          virtual LatLong::SphericalAccuracyPoint GetLatLongPoint(
              const FaceCoordinate a_coordinate) const = 0;
      };
    }
  }
}
