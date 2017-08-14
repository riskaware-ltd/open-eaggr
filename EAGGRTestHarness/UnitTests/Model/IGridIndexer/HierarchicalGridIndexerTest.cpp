//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file HierarchicalGridIndexerTest.cpp
/// 
/// Tests for the EAGGR::Model::GridIndexer::HierarchicalGridIndexer class
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

#include "TestMacros.hpp"

#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "TestUtilities/KmlTestCell.hpp"
#include "Src/EAGGRException.hpp"

static const unsigned short MAX_FACE_INDEX = 19U;

using namespace EAGGR::Model;

UNIT_TEST(HierarchicalGridIndexer, GetCellResolution0)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Face coordinate with accuracy the size of the whole face
  FaceCoordinate faceCoordinate1(2, 0.0, 0.0, 1.0);

  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoordinate1);

  EXPECT_EQ("02", cell->GetCellId());

  // Face coordinate with accuracy such that the whole face is used as the resolution
  FaceCoordinate faceCoordinate2(2, 0.0, 0.0, 0.75);

  cell = indexer.GetCell(faceCoordinate2);

  EXPECT_EQ("02", cell->GetCellId());
}

UNIT_TEST(HierarchicalGridIndexer, GetCellResolution1)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Face coordinate in the central partition
  FaceCoordinate faceCoordinate1(2, 0.1, 0.1, 0.25);
  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoordinate1);
  EXPECT_EQ("020", cell->GetCellId());

  // Face coordinate in the top partition
  FaceCoordinate faceCoordinate2(2, 0.1, 0.4, 0.25);
  cell = indexer.GetCell(faceCoordinate2);
  EXPECT_EQ("021", cell->GetCellId());

  // Face coordinate in the left partition
  FaceCoordinate faceCoordinate3(2, -0.3, -0.1, 0.25);
  cell = indexer.GetCell(faceCoordinate3);
  EXPECT_EQ("022", cell->GetCellId());

  // Face coordinate in the right partition
  FaceCoordinate faceCoordinate4(2, 0.2, -0.1, 0.25);
  cell = indexer.GetCell(faceCoordinate4);
  EXPECT_EQ("023", cell->GetCellId());
}

UNIT_TEST(HierarchicalGridIndexer, GetCellResolution4)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  FaceCoordinate faceCoordinate(12, 0.0625, 0.17140086, 0.00390625);
  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoordinate);
  EXPECT_EQ("121320", cell->GetCellId());
}

UNIT_TEST(HierarchicalGridIndexer, GetFaceCoordinateResolution4)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  FaceCoordinate faceCoordinate(12, 0.0625, 0.18042196, 0.00390625);
  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoordinate);

  // Convert back to the face coordinate
  FaceCoordinate convertedFaceCoordinate = indexer.GetFaceCoordinate(*cell);
  EXPECT_NEAR(faceCoordinate.GetXOffset(), convertedFaceCoordinate.GetXOffset(), 1E-6);
  EXPECT_NEAR(faceCoordinate.GetYOffset(), convertedFaceCoordinate.GetYOffset(), 1E-6);
}

UNIT_TEST(HierarchicalGridIndexer, CreateCell)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = indexer.CreateCell("123012301");

  EXPECT_EQ("123012301", cell->GetCellId());
  EXPECT_EQ(12, cell->GetFaceIndex());
  EXPECT_EQ(7, cell->GetResolution());
  EXPECT_EQ(Grid::STANDARD, cell->GetOrientation());

  std::unique_ptr<Cell::ICell> invertedCell = indexer.CreateCell("120123");
  EXPECT_EQ(Grid::ROTATED, invertedCell->GetOrientation());

  // Test error cases
  EXPECT_THROW(indexer.CreateCell("20111"), EAGGR::EAGGRException);
  EXPECT_THROW(indexer.CreateCell("12004"), EAGGR::EAGGRException);
  EXPECT_THROW(indexer.CreateCell("XXXXX"), EAGGR::EAGGRException);
}

UNIT_TEST(HierarchicalGridIndexer, GetParents)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  Cell::HierarchicalCell cell("123123123", MAX_FACE_INDEX, 3U);
  std::vector<std::unique_ptr<Cell::ICell> > parentCells;
  indexer.GetParents(cell, parentCells);

  EXPECT_EQ("12312312", parentCells[0]->GetCellId());
}

UNIT_TEST(HierarchicalGridIndexer, GetChildren)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;

  GridIndexer::HierarchicalGridIndexer triangleIndexer(&triangleGrid, MAX_FACE_INDEX);

  Cell::HierarchicalCell cell("123012301", MAX_FACE_INDEX, 10U);
  std::vector<std::unique_ptr<Cell::ICell> > childCells;
  triangleIndexer.GetChildren(cell, childCells);

  EXPECT_EQ(4U, childCells.size());
  EXPECT_EQ("1230123010", childCells[0]->GetCellId());
  EXPECT_EQ("1230123011", childCells[1]->GetCellId());
  EXPECT_EQ("1230123012", childCells[2]->GetCellId());
  EXPECT_EQ("1230123013", childCells[3]->GetCellId());
}

UNIT_TEST(HierarchicalGridIndexer, InvalidCell)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;
  PolyhedralGlobe::Icosahedron polyhedron;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  EAGGR::TestUtilities::KmlTestCell cell("InvalidCellType");

  std::list<FaceCoordinate> vertices;

  EXPECT_THROW(indexer.GetFaceCoordinate(cell), EAGGR::EAGGRException);
  EXPECT_THROW(indexer.GetCellVertices(cell, vertices), EAGGR::EAGGRException);
}

UNIT_TEST(HierarchicalGridIndexer, GetCellVertices)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid grid;

  GridIndexer::HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = indexer.CreateCell("123");
  std::list<FaceCoordinate> vertices;

  indexer.GetCellVertices(*cell, vertices);

  EXPECT_EQ(3U, vertices.size());

  std::list<FaceCoordinate>::const_iterator vertex = vertices.begin();

  vertex = vertices.begin();
  EXPECT_EQ(0.25, vertex->GetXOffset());
  EXPECT_EQ(sqrt(3.0) / 12.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(0.0, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(0.5, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());
}
