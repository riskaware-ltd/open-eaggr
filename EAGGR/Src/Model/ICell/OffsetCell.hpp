//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file OffsetCell.hpp
/// 
/// Implements the EAGGR::Model::OffsetCell class.
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

#include "Src/Model/ICell.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Cell
    {
      /// Represents a DGGS cell that is identified by a row and column coordinate at each resolution
      class OffsetCell: virtual public ICell
      {
        public:
          /// Constructor.
          /// @param a_faceIndex The index of the polyhedron face the cell lies on.
          /// @param a_resolution The resolution level the cell belongs to.
          /// @param a_rowCoordinate The row coordinate of the cell.
          /// @param a_columnCoordinate The column coordinate of the cell.
          /// @param a_cellLocation The location of the cell on the polyhedron face.
          /// @param a_maximumFaceIndex The maximum face index value the cell can have.
          /// @throws DGGSException If the inputs do not define a valid DGGS cell.
          OffsetCell(
              const unsigned short a_faceIndex,
              const unsigned short a_resolution,
              const long a_rowCoordinate,
              const long a_columnCoordinate,
              const CellLocation a_cellLocation,
              const unsigned short a_maximumFaceIndex);

          /// Constructor
          /// @param a_cellId The cell id representing the cell data.
          /// @param a_maximumFaceIndex The maximum face index value the cell can have.
          /// @throws DGGSException If the input string is not a valid cell ID.
          OffsetCell(const DggsCellId& a_cellId, const unsigned short a_maximumFaceIndex);

          virtual DggsCellId GetCellId() const;

          virtual unsigned short GetFaceIndex() const;
          virtual unsigned short GetResolution() const;

          virtual long GetRow() const;
          virtual long GetColumn() const;

          virtual Grid::ShapeOrientation GetOrientation() const;

          virtual void SetOrientation(const Grid::ShapeOrientation a_orientation);

          virtual CellLocation GetCellLocation() const;

        private:

          static const short m_FACE_INDEX_LENGTH = 2;
          static const short m_RESOLUTION_LENGTH = 2;

          static const char m_SEPARATOR = ',';

          unsigned short m_faceIndex;
          unsigned short m_resolution;
          long m_rowCoordinate;
          long m_columnCoordinate;
          Grid::ShapeOrientation m_orientation;
          CellLocation m_cellLocation;
      };
    }
  }
}

