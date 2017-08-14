//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid Indexer
//
//------------------------------------------------------
/// @file HierarchicalGridIndexer.hpp
/// 
/// Implements the EAGGR::Model::GridIndexer::HierarchicalGridIndexer class.
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

#include "Src/Model/IGrid/IHierarchicalGrid.hpp"
#include "Src/Model/IGridIndexer.hpp"
#include "Src/Model/IPolyhedralGlobe.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace GridIndexer
    {
      /// Implements hierarchical indexing for a grid of DGGS cells.
      class HierarchicalGridIndexer: virtual public IGridIndexer
      {
        public:
          /// Constructor
          /// @param a_pGrid The grid structure to be indexed.
          /// @param a_maximumFaceIndex The maximum face index value allowed in a cell ID.
          HierarchicalGridIndexer(
              const Grid::IHierarchicalGrid* const a_pGrid,
              const unsigned short a_maximumFaceIndex);

          virtual std::unique_ptr<Cell::ICell> GetCell(const FaceCoordinate a_faceCoordinate) const;

          virtual FaceCoordinate GetFaceCoordinate(const Cell::ICell & a_cell) const;

          virtual std::unique_ptr<Cell::ICell> CreateCell(const Cell::DggsCellId& a_cellId) const;

          virtual void GetParents(
              const Cell::ICell& a_cell,
              std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const;

          virtual void GetChildren(
              const Cell::ICell& a_cell,
              std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const;

          virtual void GetCellVertices(
              const Cell::ICell & a_cell,
              std::list<FaceCoordinate>& a_cellVertices) const;

        private:
          const Grid::IHierarchicalGrid* const m_pGrid;
          const unsigned short m_maximumFaceIndex;
      };
    }
  }
}
