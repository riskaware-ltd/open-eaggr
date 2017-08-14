//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file ICell.hpp
/// 
/// Implements the EAGGR::Model::Cell::ICell class.
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

#include <string>
#include <list>

#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/IGrid/CellPartition.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Cell
    {
      typedef std::string DggsCellId;

      /// Enum for location of cell on the polyhedron face.
      enum CellLocation
      {
        FACE, EDGE, VERTEX, UNKNOWN
      };

      /// Interface for implementations of DGGS cells
      class ICell
      {
        public:
          /// Destructor
          virtual ~ICell()
          {
          }
          ;

          /// @return The Id of the DGGS cell
          virtual DggsCellId GetCellId() const = 0;

          /// @return The index of the face the DGGS cell is located on
          virtual unsigned short GetFaceIndex() const = 0;

          /// @return The resolution level of this DGGS cell
          virtual unsigned short GetResolution() const = 0;

          /// @return The orientation of this DGGS cell
          virtual Grid::ShapeOrientation GetOrientation() const = 0;

          /// Sets the orientation of the cell
          /// @param a_orientation The orientation of the cell
          virtual void SetOrientation(const Grid::ShapeOrientation a_orientation) = 0;

          /// @return The location of the cell on the face (inside, on edge, on vertex).
          virtual CellLocation GetCellLocation() const = 0;

        protected:
          /// The maximum resolution level supported by the DGGS.
          /// This is set at the level which allows for the largest long value to be written
          /// for an offset coordinate.  If the resolution is bigger than this then the
          /// coordinates are liable for overflow.  If ISEA4T is taken forward then there is no
          /// theoretical limit to the maximum resolution.
          static const unsigned int m_MAX_RESOLUTION_LEVEL = 40;
      };
    }
  }
}
