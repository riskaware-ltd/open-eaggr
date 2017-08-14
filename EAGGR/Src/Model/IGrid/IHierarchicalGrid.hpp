//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file IHierarchicalGrid.hpp
///
/// Implements the EAGGR::Model::Grid::IHierarchicalGrid class.
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

#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/CartesianPoint.hpp"
#include "Src/Model/IGrid.hpp"
#include "Src/Model/IGrid/CellPartition.hpp"
#include "Src/Model/ICell/HierarchicalCell.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      /// Interface for a hierarchical grid that partitions the polyhedral globe face.
      class IHierarchicalGrid: public IGrid
      {
        public:
          /// Destructor
          virtual ~IHierarchicalGrid()
          {
          }
          ;

          /// Finds the location of the supplied point relative to the shape being partitioned
          /// and provides information about the partition the point is located in.
          /// @param a_cellPartition The cell partition to determine the sub-cell partition for.
          /// @param a_resolution The resolution level of the shape being partitioned.
          /// @param a_locationOnFace The location of the point on the polyhedron face.
          /// @param a_pSubCellPartition Output variable for the sub-cell partition that the point is located in.
          virtual void
          GetFacePartition(
              const CellPartition a_cellPartition,
              const short a_resolution,
              const FaceCoordinate a_locationOnFace,
              CellPartition* a_pSubCellPartition) const = 0;

          /// Finds the offset of the supplied cell from the centre of the polyhedron face
          /// @param a_cell The cell to get the location on the face for.
          /// @param a_xOffset Output variable for the x offset of the cell as a fraction of the whole face.
          /// @param a_yOffset Output variable for the y offset of the cell as a fraction of the whole face.
          virtual void
          GetFaceOffset(
              const Cell::HierarchicalCell & a_cell,
              double &a_xOffset,
              double &a_yOffset) const = 0;

          /// Gets the orientation of the supplied cell.
          /// @param a_cell The cell to get the orientation for.
          /// @return The orientation of the cell.
          virtual Grid::ShapeOrientation GetOrientation(
              const Cell::HierarchicalCell & a_cell) const = 0;
      };
    }
  }
}
