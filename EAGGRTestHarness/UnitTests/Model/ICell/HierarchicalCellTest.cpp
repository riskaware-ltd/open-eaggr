//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file HierarchicalCellTest.cpp
/// 
/// Tests for the EAGGR::Model::HierarchicalCell class
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

#include "Src/Model/ICell/HierarchicalCell.hpp"
#include "Src/EAGGRException.hpp"

static const unsigned short MAX_FACE_INDEX = 19U;

using namespace EAGGR::Model::Cell;

UNIT_TEST(HierarchicalCell, CellId)
{
  std::vector<unsigned short> cellIndices;
  cellIndices.push_back(6U);
  cellIndices.push_back(7U);
  cellIndices.push_back(8U);
  cellIndices.push_back(9U);
  cellIndices.push_back(0U);
  HierarchicalCell cell(1, cellIndices, MAX_FACE_INDEX, 10);

  EXPECT_EQ(1, cell.GetFaceIndex());
  EXPECT_EQ(5, cell.GetResolution());
  EXPECT_EQ(FACE, cell.GetCellLocation());

  EXPECT_EQ("0167890", cell.GetCellId());
}

UNIT_TEST(HierarchicalCell, Orientation)
{
  std::vector<unsigned short> cellIndices;
  cellIndices.push_back(6U);
  cellIndices.push_back(7U);
  cellIndices.push_back(8U);
  cellIndices.push_back(9U);
  cellIndices.push_back(0U);
  HierarchicalCell cell(1, cellIndices, MAX_FACE_INDEX, 10);
  EXPECT_EQ(EAGGR::Model::Grid::STANDARD, cell.GetOrientation());

  cell.SetOrientation(EAGGR::Model::Grid::ROTATED);
  EXPECT_EQ(EAGGR::Model::Grid::ROTATED, cell.GetOrientation());
}

UNIT_TEST(HierarchicalCell, Resolution0)
{
  std::vector<unsigned short> cellIndices;
  HierarchicalCell cell(1, cellIndices, MAX_FACE_INDEX, 0);

  EXPECT_EQ(1, cell.GetFaceIndex());
  EXPECT_EQ(0, cell.GetResolution());

  EXPECT_EQ("01", cell.GetCellId());
}

UNIT_TEST(HierarchicalCell, ResolutionTooLarge)
{
  std::vector<unsigned short> cellIndices;
  for (int i = 0; i < 41; ++i)
  {
    cellIndices.push_back(0U);
  }

  EXPECT_THROW(HierarchicalCell(1, cellIndices, 20, 1), EAGGR::EAGGRException);

  EXPECT_THROW(HierarchicalCell("0012345678901234567890123456789012345678901", 20, 10), EAGGR::EAGGRException);
}

UNIT_TEST(HierarchicalCell, ConstructFromCellId)
{
  HierarchicalCell cell("123456789", MAX_FACE_INDEX, 10);

  EXPECT_EQ("123456789", cell.GetCellId());
  EXPECT_EQ(12, cell.GetFaceIndex());
  EXPECT_EQ(7, cell.GetResolution());
  EXPECT_EQ(3, cell.GetCellIndex(1));
  EXPECT_EQ(4, cell.GetCellIndex(2));
  EXPECT_EQ(5, cell.GetCellIndex(3));
  EXPECT_EQ(6, cell.GetCellIndex(4));
  EXPECT_EQ(7, cell.GetCellIndex(5));
  EXPECT_EQ(8, cell.GetCellIndex(6));
  EXPECT_EQ(9, cell.GetCellIndex(7));
  EXPECT_EQ(FACE, cell.GetCellLocation());
}

UNIT_TEST(HierarchicalCell, OutOfRangeIndices)
{
  EXPECT_THROW(HierarchicalCell cell("123456789", MAX_FACE_INDEX, 5), EAGGR::EAGGRException);
  EXPECT_THROW(HierarchicalCell cell("200000000", MAX_FACE_INDEX, 5), EAGGR::EAGGRException);
  EXPECT_THROW(HierarchicalCell cell("XXXXXXXXX", MAX_FACE_INDEX, 5), EAGGR::EAGGRException);
}

