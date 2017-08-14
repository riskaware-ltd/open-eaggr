//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Polyhedral Globe
//
//------------------------------------------------------
/// @file Icosahedron.hpp
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

#pragma once

#include "Src/Model/IPolyhedralGlobe.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace PolyhedralGlobe
    {
      /// Implements the IPolyhedralGlobe interface for an Icosahedron.
      /// @details Constant values are taken from "An Equal-Area Projection for Polyhedral Globes",
      ///          John P Snyder, Cartographica Vol. 29 No 1, Spring 1992, pp. 10-21.
      /// @note    Faces of the globe are numbered as per the paper, but indexed from 0 instead of 1.
      class Icosahedron: public IPolyhedralGlobe
      {
        public:

          /// Destructor
          virtual ~Icosahedron()
          {
          }
          ;

          // Methods implementing IPolyhedralGlobe
          virtual FaceIndex GetNoOfFaces() const;

          virtual Utilities::Maths::Radians Get_g() const;
          virtual Utilities::Maths::Radians Get_G() const;
          virtual Utilities::Maths::Radians GetTheta() const;

          virtual double GetRPrimeRelativeToR() const;
          virtual double GetEdgeLengthRelativeToRPrime() const;

          virtual Utilities::Maths::Radians GetOrientationOfFace(const FaceIndex a_faceIndex) const;
          virtual LatLong::Point GetFaceCentre(const FaceIndex a_faceIndex) const;
      };
    }
  }
}
