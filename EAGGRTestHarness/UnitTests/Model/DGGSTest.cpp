//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file DGGSTest.cpp
/// 
/// Tests for the EAGGR::Model::DGGS class.
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

#include "TestMacros.hpp"

#include "Src/Model/DGGS.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IProjection/Snyder.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/ICell/HierarchicalCell.hpp"
#include "Src/Model/ICell/OffsetCell.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Cell;

static const unsigned short MAX_FACE_INDEX = 19U;
const unsigned short MAX_CELL_INDEX = 4U;

UNIT_TEST(DGGS, GetSiblingsISEA4T)
{
  PolyhedralGlobe::Icosahedron icosahedron;
  Projection::Snyder projection(&icosahedron);
  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;
  GridIndexer::HierarchicalGridIndexer gridIndexer(&triangleGrid,icosahedron.GetNoOfFaces() - 1U);
  EAGGR::Model::DGGS instanceOfDGGS(&projection, &gridIndexer);

  std::vector<std::unique_ptr<Cell::ICell> > siblingCells;

  std::vector<unsigned short> cellIndices;

  cellIndices.push_back(0U);
  cellIndices.push_back(1U);
  cellIndices.push_back(2U);
  cellIndices.push_back(3U);
  cellIndices.push_back(0U);

  EAGGR::Model::Cell::HierarchicalCell cell0(1, cellIndices, MAX_FACE_INDEX, MAX_CELL_INDEX);

  instanceOfDGGS.GetSiblings(cell0,siblingCells);

  EXPECT_EQ(3U,siblingCells.size());
  EXPECT_EQ("0101231", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("0101232", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("0101233", (*siblingCells.at(2)).GetCellId());

  siblingCells.clear();

  // Replaces last cell index
  cellIndices.pop_back();
  cellIndices.push_back(1U);

  EAGGR::Model::Cell::HierarchicalCell cell1(1, cellIndices, MAX_FACE_INDEX, MAX_CELL_INDEX);

  instanceOfDGGS.GetSiblings(cell1,siblingCells);

  EXPECT_EQ(3U,siblingCells.size());
  EXPECT_EQ("0101230", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("0101232", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("0101233", (*siblingCells.at(2)).GetCellId());

  siblingCells.clear();

  // Replaces last cell index
  cellIndices.pop_back();
  cellIndices.push_back(2U);

  EAGGR::Model::Cell::HierarchicalCell cell2(1, cellIndices, MAX_FACE_INDEX, MAX_CELL_INDEX);

  instanceOfDGGS.GetSiblings(cell2,siblingCells);

  EXPECT_EQ(3U,siblingCells.size());
  EXPECT_EQ("0101230", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("0101231", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("0101233", (*siblingCells.at(2)).GetCellId());

  siblingCells.clear();

  // Replaces last cell index
  cellIndices.pop_back();
  cellIndices.push_back(3U);

  EAGGR::Model::Cell::HierarchicalCell cell3(1, cellIndices, MAX_FACE_INDEX, MAX_CELL_INDEX);

  instanceOfDGGS.GetSiblings(cell3,siblingCells);

  EXPECT_EQ(3U,siblingCells.size());
  EXPECT_EQ("0101230", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("0101231", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("0101232", (*siblingCells.at(2)).GetCellId());
}

UNIT_TEST(DGGS, GetSiblingsISEA3H)
{
  PolyhedralGlobe::Icosahedron icosahedron;
  Projection::Snyder projection(&icosahedron);
  Grid::OffsetGrid::Aperture3HexagonGrid hexagonGrid;
  GridIndexer::OffsetGridIndexer gridIndexer(&hexagonGrid,icosahedron.GetNoOfFaces() - 1U);
  EAGGR::Model::DGGS instanceOfDGGS(&projection, &gridIndexer);

  std::vector<std::unique_ptr<Cell::ICell> > siblingCells;

  // Creates a cell on a vertex of the parent cell
  EAGGR::Model::Cell::OffsetCell cell0(1, 5, 4, 2, VERTEX, MAX_FACE_INDEX);

  instanceOfDGGS.GetSiblings(cell0,siblingCells);

  EXPECT_EQ(15U,siblingCells.size());
  EXPECT_EQ("01054,3", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("01053,3", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("01054,4", (*siblingCells.at(2)).GetCellId());
  EXPECT_EQ("01055,3", (*siblingCells.at(3)).GetCellId());
  EXPECT_EQ("01055,4", (*siblingCells.at(4)).GetCellId());
  EXPECT_EQ("01055,2", (*siblingCells.at(5)).GetCellId());
  EXPECT_EQ("01054,1", (*siblingCells.at(6)).GetCellId());
  EXPECT_EQ("01053,1", (*siblingCells.at(7)).GetCellId());
  EXPECT_EQ("01055,1", (*siblingCells.at(8)).GetCellId());
  EXPECT_EQ("01054,0", (*siblingCells.at(9)).GetCellId());
  EXPECT_EQ("01055,0", (*siblingCells.at(10)).GetCellId());
  EXPECT_EQ("01053,2", (*siblingCells.at(11)).GetCellId());
  EXPECT_EQ("01052,2", (*siblingCells.at(12)).GetCellId());
  EXPECT_EQ("01052,3", (*siblingCells.at(13)).GetCellId());
  EXPECT_EQ("01052,1", (*siblingCells.at(14)).GetCellId());

  siblingCells.clear();

  // Creates a cell inside the parent cell
  EAGGR::Model::Cell::OffsetCell cell1(1, 5, 3, 2, VERTEX, MAX_FACE_INDEX);

  instanceOfDGGS.GetSiblings(cell1,siblingCells);

  EXPECT_EQ(6U,siblingCells.size());
  EXPECT_EQ("01052,2", (*siblingCells.at(0)).GetCellId());
  EXPECT_EQ("01053,3", (*siblingCells.at(1)).GetCellId());
  EXPECT_EQ("01054,2", (*siblingCells.at(2)).GetCellId());
  EXPECT_EQ("01053,1", (*siblingCells.at(3)).GetCellId());
  EXPECT_EQ("01052,3", (*siblingCells.at(4)).GetCellId());
  EXPECT_EQ("01052,1", (*siblingCells.at(5)).GetCellId());
}
