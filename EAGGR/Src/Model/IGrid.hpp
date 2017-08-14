//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file IGrid.hpp
///
/// Implements the EAGGR::Model::Grid::IGrid class.
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
    namespace Grid
    {
      /// Interface for a grid that partitions the polyhedral globe face.
      class IGrid
      {
        public:
          /// Destructor
          virtual ~IGrid()
          {
          }
          ;

          /// Finds the resolution that has a cell size closest to the specified accuracy.
          /// @param a_accuracy The accuracy as a fraction of the face area.
          /// @return The resolution level corresponding to the specified accuracy.
          virtual unsigned short
          GetResolutionFromAccuracy(const double a_accuracy) const = 0;

          /// Finds the accuracy corresponding to the cell size at the specified resolution.
          /// @param a_resolution The resolution level to get the accuracy for.
          /// @return The accuracy as a fraction of the face area.
          virtual double GetAccuracyFromResolution(const unsigned short a_resolution) const = 0;

          /// @return The number of child cells for each parent cell in the grid.
          virtual unsigned short GetNumChildren() const = 0;

          /// @return The maximum allowed cell index for this grid.
          virtual unsigned short GetMaximumCellIndex() const = 0;

          /// @return The aperture of the grid at successive resolutions
          virtual unsigned short GetAperture() const = 0;

          /// Gets the vertices of the specified DGGS cell.
          /// @param a_cell The cell to get the vertices for.
          /// @param a_cellVertices A list that will be populated with the cell vertices
          virtual void GetVertices(
              const Cell::ICell & a_cell,
              std::list<FaceCoordinate>& a_cellVertices) const = 0;
      };
    }
  }
}
