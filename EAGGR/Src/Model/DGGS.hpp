//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file DGGS.hpp
/// 
/// Implements the EAGGR::Model::DGGS class.
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
#include "Src/Model/ICell.hpp"
#include "Src/Model/IProjection.hpp"
#include "Src/Model/IGridIndexer.hpp"

namespace EAGGR
{
  namespace Model
  {
    /// Combines an equal-area projection and grid indexer to form a DGGS.
    class DGGS
    {
      public:
        /// Specifies the projection and the grid indexer for the DGGS.
        DGGS(
            const Projection::IProjection * a_projection,
            const GridIndexer::IGridIndexer * a_gridIndexer);

        /// Creates a cell in the DGGS using a cell Id.
        std::unique_ptr<Cell::ICell> CreateCell(const Cell::DggsCellId & a_cellId) const;

        /// Converts a lat / long point to a cell in the DGGS.
        std::unique_ptr<Cell::ICell> ConvertLatLongPointToCell(
            const LatLong::SphericalAccuracyPoint a_point) const;

        /// Converts a cell in the DGGS to a lat / long point.
        LatLong::SphericalAccuracyPoint ConvertCellToLatLongPoint(const Cell::ICell & a_cell) const;

        /// Populates a_parentCells with the parent cells of the given cell.
        void GetParents(
            const Cell::ICell& a_cell,
            std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const;

        /// Populates a_childCells with the child cells of the given cell.
        void GetChildren(
            const Cell::ICell& a_cell,
            std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const;

        /// Populates a_siblingCells with the sibling cells of the given cell.
        void GetSiblings(
            const Cell::ICell& a_cell,
            std::vector<std::unique_ptr<Cell::ICell> >& a_siblingCells) const;

        /// Gets the vertices for the supplied cell
        void GetCellVertices(
            const Cell::ICell& a_cell,
            std::vector<LatLong::SphericalAccuracyPoint>& a_cellVertices) const;

      private:
        /// Projection to use for transforming points to and from the cells in
        /// the DGGS.
        const Projection::IProjection * m_projection;

        /// Grid indexer for obtaining the index of cells on the faces of the
        /// projection's polyhedral globe.
        const GridIndexer::IGridIndexer * m_gridIndexer;
    };

  }
}
