//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file CellPartition.cpp
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

#include "CellPartition.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      CellPartition::CellPartition(
          const unsigned short a_partitionId,
          const CartesianPoint &a_partitionCentre,
          const ShapeOrientation a_partitionOrientation)
          :
              m_partitionId(a_partitionId),
              m_partitionCentre(a_partitionCentre),
              m_partitionOrientation(a_partitionOrientation)
      {
      }

      unsigned short CellPartition::GetId() const
      {
        return m_partitionId;
      }

      void CellPartition::SetId(const unsigned short a_id)
      {
        m_partitionId = a_id;
      }

      CartesianPoint CellPartition::GetPartitionCentre() const
      {
        return m_partitionCentre;
      }

      void CellPartition::SetPartitionCentre(const CartesianPoint &a_partitionCentre)
      {
        m_partitionCentre = a_partitionCentre;
      }

      ShapeOrientation CellPartition::GetPartitionOrientation() const
      {
        return m_partitionOrientation;
      }

      void CellPartition::SetPartitionOrientation(ShapeOrientation a_partitionOrientation)
      {
        m_partitionOrientation = a_partitionOrientation;
      }
    }
  }
}
