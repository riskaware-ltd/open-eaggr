//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file OffsetCellTest.cpp
/// 
/// Tests for the EAGGR::Model::OffsetCell class
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

#include "Src/Model/ICell/OffsetCell.hpp"
#include "Src/EAGGRException.hpp"

static const unsigned short MAX_FACE_INDEX = 19U;

using namespace EAGGR::Model::Cell;

UNIT_TEST(OffsetCell, CellId)
{
  OffsetCell cell(1, 5, 123, 456, VERTEX, MAX_FACE_INDEX);

  EXPECT_EQ(1, cell.GetFaceIndex());
  EXPECT_EQ(5, cell.GetResolution());
  EXPECT_EQ(VERTEX, cell.GetCellLocation());

  EXPECT_EQ("0105123,456", cell.GetCellId());

  OffsetCell cellNegativeCoord(10, 5, -123, -456, UNKNOWN, MAX_FACE_INDEX);

  EXPECT_EQ(10, cellNegativeCoord.GetFaceIndex());
  EXPECT_EQ(5, cellNegativeCoord.GetResolution());

  EXPECT_EQ("1005-123,-456", cellNegativeCoord.GetCellId());
}

UNIT_TEST(OffsetCell, Orientation)
{
  OffsetCell cell(1, 5, 123, 456, UNKNOWN, MAX_FACE_INDEX);
  EXPECT_EQ(EAGGR::Model::Grid::STANDARD, cell.GetOrientation());

  cell.SetOrientation(EAGGR::Model::Grid::ROTATED);
  EXPECT_EQ(EAGGR::Model::Grid::ROTATED, cell.GetOrientation());
}

UNIT_TEST(OffsetCell, ConstructFromCellId)
{
  OffsetCell cell("0105123,456", MAX_FACE_INDEX);

  EXPECT_EQ("0105123,456", cell.GetCellId());
  EXPECT_EQ(1, cell.GetFaceIndex());
  EXPECT_EQ(5, cell.GetResolution());

  OffsetCell cellNegativeCoord("1005-123,-456", MAX_FACE_INDEX);

  EXPECT_EQ("1005-123,-456", cellNegativeCoord.GetCellId());
  EXPECT_EQ(10, cellNegativeCoord.GetFaceIndex());
  EXPECT_EQ(5, cellNegativeCoord.GetResolution());

  // Test handling of badly formed cell IDs
  EXPECT_THROW(OffsetCell cell("XX05000,000", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("01XX000,000", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("2005", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("2005,", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("0105XXX,000", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("0105000,XXX", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("0105000000", MAX_FACE_INDEX), EAGGR::EAGGRException);
  EXPECT_THROW(OffsetCell cell("2005000,,000", MAX_FACE_INDEX), EAGGR::EAGGRException);
}

UNIT_TEST(OffsetCell, FaceIndexTooLarge)
{
  EXPECT_THROW(OffsetCell(20, 5, 0, 0, UNKNOWN, MAX_FACE_INDEX), EAGGR::EAGGRException);

  EXPECT_THROW(OffsetCell("20050,0", MAX_FACE_INDEX), EAGGR::EAGGRException);
}

UNIT_TEST(OffsetCell, ResolutionTooLarge)
{
  EXPECT_THROW(OffsetCell(1, 41, 0, 0, UNKNOWN, MAX_FACE_INDEX), EAGGR::EAGGRException);

  EXPECT_THROW(OffsetCell("00410,0", MAX_FACE_INDEX), EAGGR::EAGGRException);
}
