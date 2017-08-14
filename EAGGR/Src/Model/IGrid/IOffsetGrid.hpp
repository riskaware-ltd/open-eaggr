//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file IOffsetGrid.hpp
///
/// Implements the EAGGR::Model::IOffsetGrid class.
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

#include <vector>

#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/CartesianPoint.hpp"
#include "Src/Model/IGrid.hpp"
#include "Src/Model/ICell/OffsetCell.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      /// Structure to represent a cell in an offset coordinate system.
      struct OffsetCoordinate
      {
          long m_rowId;
          long m_columnId;
      };

      /// Interface for an offset grid that partitions the polyhedral globe face.
      class IOffsetGrid: public IGrid
      {
        public:
          /// Destructor
          virtual ~IOffsetGrid()
          {
          }
          ;

          /// Gets the row and column of the cell representing the supplied face coordinate at the given resolution.
          /// @param a_resolution The resolution to get the cell at.
          /// @param a_locationOnFace The location of the point on the polyhedron face.
          /// @param a_rowId Output variable to contain the row coordinate of the cell.
          /// @param a_columnId Output variable to contain the column coordinate of the cell.
          virtual void
          GetRowAndColumn(
              const unsigned short a_resolution,
              const FaceCoordinate a_locationOnFace,
              long &a_rowId,
              long &a_columnId) const = 0;

          /// Finds the offset of the cell defined by the row, column and resolution
          /// from the centre of the polyhedron face.
          /// @param a_cell The cell to get the offset for.
          /// @param a_xOffset Output variable for the x offset of the cell as a fraction of the whole face.
          /// @param a_yOffset Output variable for the y offset of the cell as a fraction of the whole face.
          virtual void
          GetFaceOffset(
              const Cell::OffsetCell & a_cell,
              double &a_xOffset,
              double &a_yOffset) const = 0;

          /// Gets the parent cells for the cell defined by the row, column and resolution.
          /// @param a_cell The cell to get the parents for.
          /// @param a_parents A vector that will be populated with the parent cell Ids.
          virtual void GetParents(
              const Cell::OffsetCell & a_cell,
              std::vector<Grid::OffsetCoordinate>& a_parents) const = 0;

          /// Gets the child cells for the cell defined by the row, column and resolution.
          /// @param a_cell The cell to get the children for.
          /// @param a_children A vector that will be populated with the child cell Ids.
          virtual void GetChildren(
              const Cell::OffsetCell & a_cell,
              std::vector<Grid::OffsetCoordinate>& a_children) const = 0;

          /// Gets the orientation of the supplied cell.
          /// @param a_cell The cell to get the orientation for.
          /// @return The orientation of the cell.
          virtual Grid::ShapeOrientation GetOrientation(const Cell::OffsetCell & a_cell) const = 0;
      };
    }
  }
}
