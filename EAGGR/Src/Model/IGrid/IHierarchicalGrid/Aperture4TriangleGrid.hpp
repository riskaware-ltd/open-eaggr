//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file Aperture4TriangleGrid.hpp
///
/// Implements the EAGGR::Model::Grid::HierarchicalGrid::Aperture4TriangleGrid class.
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

#include <cmath>

#include "Src/Model/IGrid/IHierarchicalGrid.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      namespace HierarchicalGrid
      {
        /// Represents an aperture 4 triangular grid
        class Aperture4TriangleGrid: public IHierarchicalGrid
        {
          public:
            virtual unsigned short
            GetResolutionFromAccuracy(const double a_accuracy) const;

            virtual double GetAccuracyFromResolution(const unsigned short a_resolution) const;

            virtual void
            GetFacePartition(
                const CellPartition a_cellPartition,
                const short a_resolution,
                const FaceCoordinate a_locationOnFace,
                CellPartition* a_pSubCellPartition) const;

            virtual void
            GetFaceOffset(
                const Cell::HierarchicalCell & a_cell,
                double &a_xOffset,
                double &a_yOffset) const;

            virtual unsigned short GetNumChildren() const;

            virtual unsigned short GetMaximumCellIndex() const;

            virtual unsigned short GetAperture() const;

            virtual void GetVertices(
                const Cell::ICell & a_cell,
                std::list<FaceCoordinate>& a_cellVertices) const;

            virtual ShapeOrientation GetOrientation(const Cell::HierarchicalCell & a_cell) const;

          private:
            static constexpr double m_APERTURE = 4.0;

            static const double m_HEIGHT_TO_EDGE_RATIO;
        };
      }
    }
  }
}
