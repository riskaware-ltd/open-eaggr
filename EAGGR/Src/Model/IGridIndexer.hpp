//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file IGridIndexer.hpp
/// 
/// Implements the EAGGR::Model::IGridIndexer class.
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

#include <memory>
#include <vector>

#include "Src/Model/ICell.hpp"
#include "Src/Model/FaceCoordinate.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace GridIndexer
    {
      /// Interface for indexing systems for grids of cells that partition the polyhedral globe face.
      class IGridIndexer
      {
        public:
          /// Destructor
          virtual ~IGridIndexer()
          {
          }
          ;

          /// Gets the cell at the location specified by the coordinate
          /// @param a_faceCoordinate The location to find the cell at
          /// @return The cell object represention the specified location
          virtual std::unique_ptr<Cell::ICell> GetCell(
              const FaceCoordinate a_faceCoordinate) const = 0;

          /// Gets the coordinate on the polyhedron face represented by the specified cell
          /// @param a_cell The cell to get the location for
          /// @return The coordinate on the face representing the cell
          virtual FaceCoordinate GetFaceCoordinate(const Cell::ICell & a_cell) const = 0;

          /// Creates a cell from the cell id
          /// @param a_cellId The string representation of the cell
          /// @return The cell object created from the id
          virtual std::unique_ptr<Cell::ICell> CreateCell(
              const Cell::DggsCellId& a_cellId) const = 0;

          /// Gets the parent cells for the specified cell
          /// @param a_cell The cell to get the parents for
          /// @param a_parentCells A vector that will be populated with the parent cells
          virtual void GetParents(
              const Cell::ICell& a_cell,
              std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const = 0;

          /// Gets the child cells for the specified cell
          /// @param a_cell The cell to get the children for
          /// @param a_childCells A vector that will be populated with the child cells
          virtual void GetChildren(
              const Cell::ICell& a_cell,
              std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const = 0;

          /// Gets the vertices for the supplied cell
          /// @param a_cell The cell to get the vertices for
          /// @param a_cellVertices A list that will be populated with the cell vertices
          virtual void GetCellVertices(
              const Cell::ICell & a_cell,
              std::list<FaceCoordinate>& a_cellVertices) const = 0;
      };
    }
  }
}
