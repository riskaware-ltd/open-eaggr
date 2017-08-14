//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file TriangularFace.cpp
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

#include <cmath>

#include "TriangularFace.hpp"
#include "Src/Utilities/RadianMacros.hpp"

namespace EAGGR
{
  namespace Model
  {
    TriangularFace::TriangularFace()
        :
            m_vertex1(0.0, sqrt(3.0) / 3.0),
            m_vertex2(-0.5, -sqrt(3.0) / 6.0),
            m_vertex3(0.5, -sqrt(3.0) / 6.0)
    {
    }

    Cell::CellLocation TriangularFace::CalculateCellLocation(
        const FaceCoordinate a_faceCoordinate,
        const double a_cellArea) const
    {
      // Convert the point to barycentric coordinates for the triangle (http://en.wikipedia.org/wiki/Barycentric_coordinate_system)

      const double denominator = ((m_vertex2.GetY() - m_vertex3.GetY())
          * (m_vertex1.GetX() - m_vertex3.GetX()))
          + ((m_vertex3.GetX() - m_vertex2.GetX()) * (m_vertex1.GetY() - m_vertex3.GetY()));

      const double lambda1 = (((m_vertex2.GetY() - m_vertex3.GetY())
          * (a_faceCoordinate.GetXOffset() - m_vertex3.GetX()))
          + ((m_vertex3.GetX() - m_vertex2.GetX())
              * (a_faceCoordinate.GetYOffset() - m_vertex3.GetY()))) / denominator;

      const double lambda2 = (((m_vertex3.GetY() - m_vertex1.GetY())
          * (a_faceCoordinate.GetXOffset() - m_vertex3.GetX()))
          + ((m_vertex1.GetX() - m_vertex3.GetX())
              * (a_faceCoordinate.GetYOffset() - m_vertex3.GetY()))) / denominator;

      const double lambda3 = 1.0 - lambda1 - lambda2;

      // Due to floating point comparison need a tolerance to ensure we capture cell centres
      // that don't exactly fall on the edge or vertex.  Assume the cell area is a circle and set
      // the tolerance to half the radius.  This is a bit arbitrary but will ensure that cells that
      // are not on the edge/vertex are not within the tolerance but small floating-point
      // inaccuracies do not prevent the actual cells on the edge/vertex from being picked up.
      const double tolerance = sqrt(a_cellArea / PI) / 2.0;

      if ((lambda1 > 1.0 - tolerance) || (lambda2 > 1.0 - tolerance) || (lambda3 > 1.0 - tolerance))
      {
        // If one of the lambdas is 1 then we are on the vertex
        return Cell::VERTEX;
      }
      else if ((lambda1 < tolerance) || (lambda2 < tolerance) || (lambda3 < tolerance))
      {
        // If one of the lambdas is 0 then we are on an edge
        return Cell::EDGE;
      }
      else
      {
        // Otherwise we are inside the face
        return Cell::FACE;
      }
    }
  }
}
