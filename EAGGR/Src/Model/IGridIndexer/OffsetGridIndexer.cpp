//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file OffsetGridIndexer.cpp
/// 
/// Implements the EAGGR::Model::GridIndexer::OffsetGridIndexer class.
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

#include <cmath>

#include "OffsetGridIndexer.hpp"
#include "Src/Model/ICell/OffsetCell.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace GridIndexer
    {
      OffsetGridIndexer::OffsetGridIndexer(
          const Grid::IOffsetGrid* a_pGrid,
          const unsigned short a_maximumFaceIndex)
          : m_pGrid(a_pGrid), m_maximumFaceIndex(a_maximumFaceIndex), m_face()
      {
      }

      std::unique_ptr<Cell::ICell> OffsetGridIndexer::GetCell(
          const FaceCoordinate a_faceCoordinate) const
      {
        // Calculate the resolution
        const unsigned short resolution = m_pGrid->GetResolutionFromAccuracy(
            a_faceCoordinate.GetAccuracy());

        long rowId;
        long columnId;

        m_pGrid->GetRowAndColumn(resolution, a_faceCoordinate, rowId, columnId);

        return std::unique_ptr < Cell::ICell
            > (new Cell::OffsetCell(
                a_faceCoordinate.GetFaceIndex(),
                resolution,
                rowId,
                columnId,
                m_face.CalculateCellLocation(
                    a_faceCoordinate,
                    m_pGrid->GetAccuracyFromResolution(resolution)),
                m_maximumFaceIndex));
      }

      FaceCoordinate OffsetGridIndexer::GetFaceCoordinate(const Cell::ICell & a_cell) const
      {
        const Cell::OffsetCell &cell = dynamic_cast<const Cell::OffsetCell&>(a_cell);

        const unsigned short resolution = cell.GetResolution();

        double xOffset;
        double yOffset;

        m_pGrid->GetFaceOffset(cell, xOffset, yOffset);

        const double accuracy = m_pGrid->GetAccuracyFromResolution(resolution);

        return FaceCoordinate(a_cell.GetFaceIndex(), xOffset, yOffset, accuracy);
      }

      std::unique_ptr<Cell::ICell> OffsetGridIndexer::CreateCell(
          const Cell::DggsCellId& a_cellId) const
      {
        return std::unique_ptr < Cell::ICell > (new Cell::OffsetCell(a_cellId, m_maximumFaceIndex));
      }

      void OffsetGridIndexer::GetParents(
          const Cell::ICell& a_cell,
          std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const
      {
        a_parentCells.clear();

        const Cell::OffsetCell& offsetCell = dynamic_cast<const Cell::OffsetCell&>(a_cell);

        std::vector < Grid::OffsetCoordinate > parents;
        m_pGrid->GetParents(offsetCell, parents);

        for (unsigned short parent = 0; parent < parents.size(); ++parent)
        {
          /// @todo: Need to re-think this if ISEA3H is taken forward - parent cell location != child location
          Cell::OffsetCell* parentCell = new Cell::OffsetCell(
              a_cell.GetFaceIndex(),
              a_cell.GetResolution() - 1,
              parents.at(parent).m_rowId,
              parents.at(parent).m_columnId,
              offsetCell.GetCellLocation(),
              m_maximumFaceIndex);
          a_parentCells.push_back(std::unique_ptr < Cell::ICell > (parentCell));
        }
      }

      void OffsetGridIndexer::GetChildren(
          const Cell::ICell& a_cell,
          std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const
      {
        a_childCells.clear();

        const Cell::OffsetCell& offsetCell = dynamic_cast<const Cell::OffsetCell&>(a_cell);

        std::vector < Grid::OffsetCoordinate > childCells;
        m_pGrid->GetChildren(offsetCell, childCells);

        for (unsigned int childIndex = 0; childIndex < childCells.size(); ++childIndex)
        {
          /// @todo: Need to re-think this if ISEA3H is taken forward - parent cell location != child location
          Cell::OffsetCell* childCell = new Cell::OffsetCell(
              offsetCell.GetFaceIndex(),
              offsetCell.GetResolution() + 1,
              childCells.at(childIndex).m_rowId,
              childCells.at(childIndex).m_columnId,
              offsetCell.GetCellLocation(),
              m_maximumFaceIndex);
          a_childCells.push_back(std::unique_ptr < Cell::ICell > (childCell));
        }
      }

      void OffsetGridIndexer::GetCellVertices(
          const Cell::ICell & a_cell,
          std::list<FaceCoordinate>& a_cellVertices) const
      {
        m_pGrid->GetVertices(a_cell, a_cellVertices);
      }
    }
  }
}
