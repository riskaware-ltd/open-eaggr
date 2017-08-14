//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid Indexer
//
//------------------------------------------------------
/// @file HierarchicalGridIndexer.cpp
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

#include <cmath>
#include <sstream>
#include <typeinfo>

#include "HierarchicalGridIndexer.hpp"
#include "Src/Model/ICell/HierarchicalCell.hpp"
#include "Src/Utilities/Maths.hpp"
#include "Src/Model/IGrid/CellPartition.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace Model
  {
    namespace GridIndexer
    {
      HierarchicalGridIndexer::HierarchicalGridIndexer(
          const Grid::IHierarchicalGrid* const a_pGrid,
          const unsigned short a_maximumFaceIndex)
          : m_pGrid(a_pGrid), m_maximumFaceIndex(a_maximumFaceIndex)
      {
      }

      std::unique_ptr<Cell::ICell> HierarchicalGridIndexer::GetCell(
          const FaceCoordinate a_faceCoordinate) const
      {
        // Calculate the resolution
        const unsigned short resolution = m_pGrid->GetResolutionFromAccuracy(
            a_faceCoordinate.GetAccuracy());

        // Initial parameters
        unsigned short partitionId = m_pGrid->GetAperture(); // initialise to invalid value
        Grid::ShapeOrientation partitionOrientation = Grid::STANDARD;
        CartesianPoint partitionShapeCentre(0.0, 0.0);

        FaceCoordinate coordinateInPartition(
            a_faceCoordinate.GetFaceIndex(),
            a_faceCoordinate.GetXOffset(),
            a_faceCoordinate.GetYOffset(),
            a_faceCoordinate.GetAccuracy());

        std::vector<unsigned short> cellIndices;

        CartesianPoint partitionCentre(0.0, 0.0);
        Grid::CellPartition partition(partitionId, partitionCentre, partitionOrientation);
        // Determine the cell index at each resolution level and add to the cell
        for (unsigned short resolutionLevel = 1; resolutionLevel <= resolution; ++resolutionLevel)
        {
          m_pGrid->GetFacePartition(partition, resolutionLevel, coordinateInPartition, &partition);

          cellIndices.push_back(partition.GetId());
        }

        // Create a cell object
        Cell::HierarchicalCell* hierarchicalCell = new Cell::HierarchicalCell(
            a_faceCoordinate.GetFaceIndex(),
            cellIndices,
            m_maximumFaceIndex,
            m_pGrid->GetMaximumCellIndex());

        return std::unique_ptr < Cell::ICell > (hierarchicalCell);
      }

      FaceCoordinate HierarchicalGridIndexer::GetFaceCoordinate(const Cell::ICell & a_cell) const
      {
        try
        {
          const Cell::HierarchicalCell &cell = dynamic_cast<const Cell::HierarchicalCell&>(a_cell);

          const unsigned short faceIndex = cell.GetFaceIndex();

          const unsigned short resolution = cell.GetResolution();

          double xOffset;
          double yOffset;

          m_pGrid->GetFaceOffset(cell, xOffset, yOffset);

          const double accuracy = m_pGrid->GetAccuracyFromResolution(resolution);

          return FaceCoordinate(faceIndex, xOffset, yOffset, accuracy);
        }
        catch (std::bad_cast& e)
        {
          throw EAGGRException(
              "Cell in HierarchicalGridIndexer GetFaceCoordinate is not a hierarchical cell.");
        }
      }

      std::unique_ptr<Cell::ICell> HierarchicalGridIndexer::CreateCell(
          const Cell::DggsCellId& a_cellId) const
      {
        Cell::HierarchicalCell* cell = new Cell::HierarchicalCell(
            a_cellId,
            m_maximumFaceIndex,
            m_pGrid->GetMaximumCellIndex());

        Grid::ShapeOrientation orientation = m_pGrid->GetOrientation(*cell);
        cell->SetOrientation(orientation);

        return std::unique_ptr < Cell::ICell > (cell);
      }

      void HierarchicalGridIndexer::GetParents(
          const Cell::ICell& a_cell,
          std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const
      {
        // Ensure the output array is empty
        a_parentCells.clear();

        // Maximum cell index is needed for the HierarchicalCell constructor
        const unsigned short maximumCellIndex = m_pGrid->GetMaximumCellIndex();

        // Get the cell indices of the child cell
        const Cell::HierarchicalCell& hierarchicalCell =
            dynamic_cast<const Cell::HierarchicalCell&>(a_cell);
        std::vector<unsigned short> cellIndices = hierarchicalCell.GetCellIndices();

        // Remove the last index from the child cell to obtain the parent cell
        cellIndices.pop_back();

        // Create the new parent cell
        Cell::HierarchicalCell* parentCell = new Cell::HierarchicalCell(
            hierarchicalCell.GetFaceIndex(),
            cellIndices,
            m_maximumFaceIndex,
            maximumCellIndex);

        // Add the new cell to the output vector
        a_parentCells.push_back(std::unique_ptr < Cell::ICell > (parentCell));
      }

      void HierarchicalGridIndexer::GetChildren(
          const Cell::ICell& a_cell,
          std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const
      {
        // Ensure the output array is empty
        a_childCells.clear();

        // Maximum cell index is needed for the HierarchicalCell constructor
        const unsigned short maximumCellIndex = m_pGrid->GetMaximumCellIndex();

        // Get the cell indices of the parent cell
        const Cell::HierarchicalCell& hierarchicalCell =
            dynamic_cast<const Cell::HierarchicalCell&>(a_cell);
        std::vector<unsigned short> cellIndices = hierarchicalCell.GetCellIndices();

        // Child cells will have an extra index on the end of the parent's indices
        cellIndices.push_back(0U);
        const unsigned short newIndexPosition = cellIndices.size() - 1U;

        // Add a cell for every child
        for (int childIndex = 0; childIndex < m_pGrid->GetNumChildren(); ++childIndex)
        {
          // Update the cell indices for the new child
          cellIndices[newIndexPosition] = childIndex;

          // Create the new child cell
          Cell::HierarchicalCell* childCell = new Cell::HierarchicalCell(
              hierarchicalCell.GetFaceIndex(),
              cellIndices,
              m_maximumFaceIndex,
              maximumCellIndex);

          // Add the new cell to the output vector
          a_childCells.push_back(std::unique_ptr < Cell::ICell > (childCell));
        }
      }

      void HierarchicalGridIndexer::GetCellVertices(
          const Cell::ICell & a_cell,
          std::list<FaceCoordinate>& a_cellVertices) const
      {
        m_pGrid->GetVertices(a_cell, a_cellVertices);
      }
    }
  }
}
