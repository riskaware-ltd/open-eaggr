//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file TriangularFace.hpp
/// 
/// Implements the EAGGR::Model::TriangularFace class.
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

#include "Src/Model/CartesianPoint.hpp"
#include "Src/Model/ICell.hpp"

namespace EAGGR
{
  namespace Model
  {
    /// Represents a face of the polyhedron
    class TriangularFace
    {
      public:
        /// Constructor
        TriangularFace();

        /// Calculates the location of the cell relative to the edge or corner of the face.
        /// @param a_faceCoordinate The coordinate of the cell centre on the face.
        /// @param a_cellArea The area of the cell as a fraction of the face.
        /// @return The type of location of the cell on the face.
        Cell::CellLocation CalculateCellLocation(
            const FaceCoordinate a_faceCoordinate,
            const double a_cellArea) const;

      private:
        CartesianPoint m_vertex1;
        CartesianPoint m_vertex2;
        CartesianPoint m_vertex3;
    };
  }
}
