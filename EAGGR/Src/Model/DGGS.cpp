//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file DGGS.cpp
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

#include "DGGS.hpp"

using namespace EAGGR::Model::Cell;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::Model::Projection;

namespace EAGGR
{
  namespace Model
  {
    DGGS::DGGS(const IProjection * a_projection, const IGridIndexer * a_gridIndexer)
        : m_projection(a_projection), m_gridIndexer(a_gridIndexer)
    {
    }

    std::unique_ptr<ICell> DGGS::CreateCell(const std::string & a_cellId) const
    {
      return (m_gridIndexer->CreateCell(a_cellId));
    }

    std::unique_ptr<ICell> DGGS::ConvertLatLongPointToCell(
        const LatLong::SphericalAccuracyPoint a_point) const
    {
      const FaceCoordinate faceCoord = m_projection->GetFaceCoordinate(a_point);

      return (m_gridIndexer->GetCell(faceCoord));
    }

    LatLong::SphericalAccuracyPoint DGGS::ConvertCellToLatLongPoint(const ICell & a_cell) const
    {
      const FaceCoordinate faceCoord = m_gridIndexer->GetFaceCoordinate(a_cell);

      return (m_projection->GetLatLongPoint(faceCoord));
    }

    void DGGS::GetParents(
        const Cell::ICell& a_cell,
        std::vector<std::unique_ptr<Cell::ICell> >& a_parentCells) const
    {
      m_gridIndexer->GetParents(a_cell, a_parentCells);
    }

    void DGGS::GetChildren(
        const Cell::ICell& a_cell,
        std::vector<std::unique_ptr<Cell::ICell> >& a_childCells) const
    {
      m_gridIndexer->GetChildren(a_cell, a_childCells);
    }

    void DGGS::GetSiblings(
        const Cell::ICell& a_cell,
        std::vector<std::unique_ptr<Cell::ICell> >& a_siblingCells) const
    {

      a_siblingCells.clear();

      // Variable to store the parent cells
      std::vector < std::unique_ptr<Cell::ICell> > parentCells;

      // Retrieves the parent cells
      GetParents(a_cell, parentCells);

      // Variable to store the child cells
      std::vector < std::unique_ptr<Cell::ICell> > childCells;

      // Iterates over the parent cells, retrieving their child cells
      for (unsigned int parentIndex = 0; parentIndex < parentCells.size(); parentIndex++)
      {
        // Clears the vector that will contain the child cells
        childCells.clear();

        // Retrieves the child cells of the current parent cell
        GetChildren(*parentCells.at(parentIndex), childCells);

        // Iterates through the children of the current parent cell
        for (unsigned int childIndex = 0; childIndex < childCells.size(); childIndex++)
        {
          // Initialises a boolean that indicates whether the ID of the current child is currently within a_siblingCells
          bool uniqueChild = true;

          // Compares the ID of the current child with the ID of the cell whose siblings we require.
          // If they are equal the child is not stored in a_siblingCells
          if (childCells.at(childIndex)->GetCellId().compare(a_cell.GetCellId()) != 0)
          {
            // Checks to see if any child cells have been placed in a_siblingCells
            if (a_siblingCells.size() != 0)
            {
              // Iterates over the number of children currently present in a_siblingCells
              for (unsigned int uniqueChildIndex = 0; uniqueChildIndex < a_siblingCells.size();
                  uniqueChildIndex++)
              {
                // Checks to see if the current child cell is already in a_siblingCells
                if (childCells.at(childIndex)->GetCellId().compare(
                    a_siblingCells.at(uniqueChildIndex)->GetCellId()) == 0)
                {
                  // If the child cell is already present in a_siblingCells
                  uniqueChild = false;

                  break;
                }
              }
            }

            // If the child corresponding to childIndex is not already present in a_siblingCells
            if (uniqueChild == true)
            {
              // Moves the pointer for the current child cell so it is stored as a sibling cell
              a_siblingCells.push_back(std::move(childCells.at(childIndex)));
            }
          }
        }
      }
    }

    void DGGS::GetCellVertices(
        const Cell::ICell & a_cell,
        std::vector<LatLong::SphericalAccuracyPoint>& a_cellVertices) const
    {
      std::list < FaceCoordinate > cellFaceCoordinates;
      m_gridIndexer->GetCellVertices(a_cell, cellFaceCoordinates);

      for (std::list<FaceCoordinate>::const_iterator iter = cellFaceCoordinates.begin();
          iter != cellFaceCoordinates.end(); ++iter)
      {
        a_cellVertices.push_back(m_projection->GetLatLongPoint(*iter));
      }
    }
  }
}
