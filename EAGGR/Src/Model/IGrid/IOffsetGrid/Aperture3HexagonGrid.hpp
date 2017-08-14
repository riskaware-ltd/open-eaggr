//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file Aperture3HexagonGrid.hpp
///
/// Implements the EAGGR::Model::Grid::OffsetGrid::Aperture3HexagonGrid class.
///
/// @note This class assumes that the underlying polygon face is an equilateral triangle.
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

#include "Src/Model/IGrid/IOffsetGrid.hpp"
#include "Src/Model/FaceCoordinate.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      namespace OffsetGrid
      {
        /// Represents an aperture 3 hexagon grid
        /// @ TODO: need to deal with the pentagons at the corners of the grid
        /// @ TODO: do not currently ensure that hexagons that span faces have the same index
        class Aperture3HexagonGrid: public IOffsetGrid
        {
          public:
            Aperture3HexagonGrid()
            {
            }
            ;

            virtual unsigned short
            GetResolutionFromAccuracy(const double a_accuracy) const;

            virtual double GetAccuracyFromResolution(const unsigned short a_resolution) const;

            virtual void
            GetRowAndColumn(
                const unsigned short a_resolution,
                const FaceCoordinate a_locationOnFace,
                long &a_rowId,
                long &a_columnId) const;

            virtual void
            GetFaceOffset(
                const Cell::OffsetCell & a_cell,
                double &a_xOffset,
                double &a_yOffset) const;

            virtual void GetParents(
                const Cell::OffsetCell & a_cell,
                std::vector<Grid::OffsetCoordinate>& a_parents) const;

            virtual unsigned short GetNumChildren() const;

            virtual unsigned short GetMaximumCellIndex() const;

            virtual void GetChildren(
                const Cell::OffsetCell & a_cell,
                std::vector<Grid::OffsetCoordinate>& a_children) const;

            virtual unsigned short GetAperture() const;

            virtual void GetVertices(
                const Cell::ICell & a_cell,
                std::list<FaceCoordinate>& a_cellVertices) const;

            virtual ShapeOrientation GetOrientation(const Cell::OffsetCell & a_cell) const;

          private:
            /// Stores a cube coordinate on the hexgaonal grid.
            struct CubeCoordinate
            {
                long m_x;
                long m_y;
                long m_z;
            };

            static constexpr double m_APERTURE = 3.0;
            static const unsigned short m_ONE_PARENT = 1;
            static const unsigned short m_THREE_PARENTS = 3;

            /// Returns the cell edge length as a fraction of the face edge length
            /// for a given resolution.
            /// @note Resolution must be greater than zero.
            double GetCellEdgeLengthFromResolution(const unsigned short a_resolution) const;

            /// Returns a cube coordinate that represents the cell which contains
            /// the fractional input coordinates.
            CubeCoordinate RoundToNearestCubeCoordinate(
                const double a_x,
                const double a_y,
                const double a_z) const;

            bool IsHorizontalOrientation(const unsigned short a_resolution) const;
        };
      }
    }
  }
}
