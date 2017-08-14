//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Cell
//
//------------------------------------------------------
/// @file HierarchicalCell.hpp
/// 
/// Implements the EAGGR::Model::Cell::HierarchicalCell class.
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

#include "Src/Model/ICell.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Cell
    {
      /// Represents a DGGS cell that is identified by a hierarchy of cells at each resolution.
      class HierarchicalCell: virtual public ICell
      {
        public:
          /// Constructor
          /// @param a_faceIndex The index of the polyhedron face the cell lies on.
          /// @param a_cellIndices The cell indices - one entry for each resolution level.
          /// @param a_maximumFaceIndex The maximum allowed face index value.
          /// @param a_maximumCellIndex The maximum allowed cell index value.
          HierarchicalCell(
              const unsigned short a_faceIndex,
              const std::vector<unsigned short>& a_cellIndices,
              const unsigned short a_maximumFaceIndex,
              const unsigned short a_maximumCellIndex);

          /// Constructor
          /// @param a_cellId The cell id representing the cell data.
          /// @param a_maximumFaceIndex The maximum allowed face index value.
          /// @param a_maximumCellIndex The maximum allowed cell index value.
          HierarchicalCell(
              const DggsCellId& a_cellId,
              const unsigned short a_maximumFaceIndex,
              const unsigned short a_maximumCellIndex);

          virtual DggsCellId GetCellId() const;

          virtual unsigned short GetFaceIndex() const;
          virtual unsigned short GetResolution() const;

          /// @return Copy of the cell's indices.
          std::vector<unsigned short> GetCellIndices() const;

          /// @param a_resolutionLevel Resolution of the required cell index.
          /// @return Requested cell index.
          unsigned short GetCellIndex(const unsigned int a_resolutionLevel) const;

          virtual Grid::ShapeOrientation GetOrientation() const;

          virtual void SetOrientation(const Grid::ShapeOrientation a_orientation);

          virtual CellLocation GetCellLocation() const;

        private:

          unsigned short m_faceIndex;
          unsigned short m_resolution;
          std::vector<unsigned short> m_cellIndices;
          unsigned short m_maximumCellIndex;
          Grid::ShapeOrientation m_orientation;

          static const int m_FACE_INDEX_LENGTH = 2;
      };
    }
  }
}
