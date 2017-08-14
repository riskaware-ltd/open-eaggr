//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file CellPartition.hpp
///
/// Implements the EAGGR::Model::Grid::CellPartition class.
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

#include "Src/Model/CartesianPoint.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      /// Enumeration of shape orientations.
      enum ShapeOrientation
      {
        STANDARD, ROTATED
      };

      /// Represents a partition of a grid cell.
      class CellPartition
      {
        public:
          /// Constructor
          /// @param a_partitionId The id of the partition.
          /// @param a_partitionCentre The centre of the partition.
          /// @param a_partitionOrientation The orientation of the partition.
          CellPartition(
              const unsigned short a_partitionId,
              const CartesianPoint &a_partitionCentre,
              const ShapeOrientation a_partitionOrientation);

          /// @return The id of the partition.
          unsigned short GetId() const;

          /// Sets the id of the partition.
          /// @param a_id The id of the partition.
          void SetId(const unsigned short a_id);

          /// @return The centre of the partition.
          CartesianPoint GetPartitionCentre() const;

          /// Sets the centre of the partition.
          /// @param a_partitionCentre The centre of the partition.
          void SetPartitionCentre(const CartesianPoint &a_partitionCentre);

          /// @return The orientation of the partition.
          ShapeOrientation GetPartitionOrientation() const;

          /// Sets the orientation of the partition.
          /// @param a_partitionOrientation The orientation of the partition.
          void SetPartitionOrientation(ShapeOrientation a_partitionOrientation);

        private:
          unsigned short m_partitionId;
          CartesianPoint m_partitionCentre;
          ShapeOrientation m_partitionOrientation;
      };
    }
  }
}
