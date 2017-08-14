//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file OffsetGridIndexerTest.cpp
/// 
/// Tests for the EAGGR::Model::GridIndexer::OffsetGridIndexer class
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

#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"

static const unsigned short MAX_FACE_INDEX = 19U;

using namespace EAGGR::Model;
using namespace EAGGR::Model::Grid::OffsetGrid;
using namespace EAGGR::Model::PolyhedralGlobe;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::Model::Cell;

UNIT_TEST(OffsetGridIndexer, GetCellResolution0)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Face coordinate with accuracy the size of the whole face
  FaceCoordinate faceCoordinate1(2, 0.0, 0.0, 1.0);

  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinate1);

  EXPECT_EQ("02000,0", cell->GetCellId());

  // Face coordinate with accuracy such that the whole face is used as the resolution
  FaceCoordinate faceCoordinate2(2, 0.0, 0.0, 0.75);

  cell = indexer.GetCell(faceCoordinate2);

  EXPECT_EQ("02000,0", cell->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetCellResolution1)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Face coordinate in the central hexagon
  FaceCoordinate faceCoordinate1(2, 0.1, 0.1, 2.0 / 3.0);
  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinate1);
  EXPECT_EQ("02010,0", cell->GetCellId());

  // Face coordinate in the top of the face
  FaceCoordinate faceCoordinate2(2, 0.0, 0.57, 2.0 / 3.0);
  cell = indexer.GetCell(faceCoordinate2);
  EXPECT_EQ("02011,0", cell->GetCellId());

  // Face coordinate in the bottom left of the face
  FaceCoordinate faceCoordinate3(2, -0.45, -0.28, 2.0 / 3.0);
  cell = indexer.GetCell(faceCoordinate3);
  EXPECT_EQ("0201-1,-1", cell->GetCellId());

  // Face coordinate in the bottom right of the face
  FaceCoordinate faceCoordinate4(2, 0.45, -0.28, 2.0 / 3.0);
  cell = indexer.GetCell(faceCoordinate4);
  EXPECT_EQ("0201-1,1", cell->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetCellResolution2)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Face coordinate in the central hexagon
  FaceCoordinate faceCoordinate1(2, 0.1, 0.1, 2.0 / 9.0);
  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinate1);
  EXPECT_EQ("02020,0", cell->GetCellId());

  // Face coordinate in the top left hexagon
  FaceCoordinate faceCoordinate2(2, -0.25, 0.35, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate2);
  EXPECT_EQ("02021,-1", cell->GetCellId());

  // Face coordinate in the top right hexagon
  FaceCoordinate faceCoordinate3(2, 0.25, 0.35, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate3);
  EXPECT_EQ("02021,0", cell->GetCellId());

  // Face coordinate in the right hexagon
  FaceCoordinate faceCoordinate4(2, 0.3, 0.0, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate4);
  EXPECT_EQ("02020,1", cell->GetCellId());

  // Face coordinate in the bottom right hexagon
  FaceCoordinate faceCoordinate5(2, 0.16, -0.25, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate5);
  EXPECT_EQ("0202-1,0", cell->GetCellId());

  // Face coordinate in the bottom left hexagon
  FaceCoordinate faceCoordinate6(2, -0.16, -0.25, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate6);
  EXPECT_EQ("0202-1,-1", cell->GetCellId());

  // Face coordinate in the left hexagon
  FaceCoordinate faceCoordinate7(2, -0.3, 0.0, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinate7);
  EXPECT_EQ("02020,-1", cell->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetCellResolution3)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  FaceCoordinate faceCoordinate(12, -0.2, 0.28, 1.0 / 9.0);
  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinate);
  EXPECT_EQ("12031,-1", cell->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetFaceCoordinateResolution2)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  FaceCoordinate faceCoordinateEvenRow(12, -1.0 / 6.0, 0.28867513, 2.0 / 9.0);
  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinateEvenRow);

  // Convert back to the face coordinate
  FaceCoordinate convertedFaceCoordinateEvenRow = indexer.GetFaceCoordinate(*cell);
  EXPECT_NEAR(
      faceCoordinateEvenRow.GetXOffset(),
      convertedFaceCoordinateEvenRow.GetXOffset(),
      1E-6);
  EXPECT_NEAR(
      faceCoordinateEvenRow.GetYOffset(),
      convertedFaceCoordinateEvenRow.GetYOffset(),
      1E-6);

  FaceCoordinate faceCoordinateOddRow(12, 1.0 / 3.0, 0.0, 2.0 / 9.0);
  cell = indexer.GetCell(faceCoordinateOddRow);

  // Convert back to the face coordinate
  FaceCoordinate convertedFaceCoordinateOddRow = indexer.GetFaceCoordinate(*cell);
  EXPECT_NEAR(faceCoordinateOddRow.GetXOffset(), convertedFaceCoordinateOddRow.GetXOffset(), 1E-6);
  EXPECT_NEAR(faceCoordinateOddRow.GetYOffset(), convertedFaceCoordinateOddRow.GetYOffset(), 1E-6);
}

UNIT_TEST(OffsetGridIndexer, GetFaceCoordinateResolution3)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  FaceCoordinate faceCoordinateOddColumn(12, 1.0 / 6.0, 0.096225044, 1.0 / 9.0);
  std::unique_ptr<ICell> cell = indexer.GetCell(faceCoordinateOddColumn);

  // Convert back to the face coordinate
  FaceCoordinate convertedFaceCoordinate = indexer.GetFaceCoordinate(*cell);
  EXPECT_NEAR(faceCoordinateOddColumn.GetXOffset(), convertedFaceCoordinate.GetXOffset(), 1E-6);
  EXPECT_NEAR(faceCoordinateOddColumn.GetYOffset(), convertedFaceCoordinate.GetYOffset(), 1E-6);

  FaceCoordinate faceCoordinateEvenColumn(12, -1.0 / 3.0, -0.192450, 1.0 / 9.0);
  cell = indexer.GetCell(faceCoordinateEvenColumn);

  // Convert back to the face coordinate
  FaceCoordinate convertedFaceCoordinateEvenColumn = indexer.GetFaceCoordinate(*cell);
  EXPECT_NEAR(
      faceCoordinateEvenColumn.GetXOffset(),
      convertedFaceCoordinateEvenColumn.GetXOffset(),
      1E-6);
  EXPECT_NEAR(
      faceCoordinateEvenColumn.GetYOffset(),
      convertedFaceCoordinateEvenColumn.GetYOffset(),
      1E-6);
}

UNIT_TEST(OffsetGridIndexer, CreateCell)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  std::unique_ptr<ICell> cell = indexer.CreateCell("0105123,456");

  EXPECT_EQ("0105123,456", cell->GetCellId());
  EXPECT_EQ(1, cell->GetFaceIndex());
  EXPECT_EQ(5, cell->GetResolution());
}

UNIT_TEST(OffsetGridIndexer, GetParents)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Origin
  OffsetCell originCell("01030,0", MAX_FACE_INDEX);
  std::vector<std::unique_ptr<Cell::ICell> > cells;
  indexer.GetParents(originCell, cells);
  EXPECT_EQ(1U, cells.size());
  EXPECT_EQ("01020,0", cells[0]->GetCellId());

  // Cell with one parent
  OffsetCell oneParentCell("01033,6", MAX_FACE_INDEX);
  indexer.GetParents(oneParentCell, cells);
  EXPECT_EQ(1U, cells.size());
  EXPECT_EQ("01022,3", cells[0]->GetCellId());

  // Cell with 3 parents
  OffsetCell threeParentCell("01035,2", MAX_FACE_INDEX);
  indexer.GetParents(threeParentCell, cells);
  EXPECT_EQ(3U, cells.size());
  EXPECT_EQ("01023,1", cells[0]->GetCellId());
  EXPECT_EQ("01024,1", cells[1]->GetCellId());
  EXPECT_EQ("01023,0", cells[2]->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetChildren)
{
  Aperture3HexagonGrid grid;

  OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  // Origin
  OffsetCell originCell("01030,0", MAX_FACE_INDEX);
  std::vector<std::unique_ptr<Cell::ICell> > cells;
  indexer.GetChildren(originCell, cells);
  EXPECT_EQ(7U, cells.size());
  EXPECT_EQ("01040,0", cells[0]->GetCellId());
  EXPECT_EQ("0104-1,0", cells[1]->GetCellId());
  EXPECT_EQ("01040,1", cells[2]->GetCellId());
  EXPECT_EQ("01041,0", cells[3]->GetCellId());
  EXPECT_EQ("01040,-1", cells[4]->GetCellId());
  EXPECT_EQ("01041,-1", cells[5]->GetCellId());
  EXPECT_EQ("0104-1,-1", cells[6]->GetCellId());

  // Cell away from origin
  OffsetCell nonOriginCell("01031,3", MAX_FACE_INDEX);
  indexer.GetChildren(nonOriginCell, cells);
  EXPECT_EQ(7U, cells.size());
  EXPECT_EQ("01043,4", cells[0]->GetCellId());
  EXPECT_EQ("01042,4", cells[1]->GetCellId());
  EXPECT_EQ("01043,5", cells[2]->GetCellId());
  EXPECT_EQ("01044,4", cells[3]->GetCellId());
  EXPECT_EQ("01043,3", cells[4]->GetCellId());
  EXPECT_EQ("01042,5", cells[5]->GetCellId());
  EXPECT_EQ("01044,5", cells[6]->GetCellId());
}

UNIT_TEST(OffsetGridIndexer, GetCellVertices)
{
  Aperture3HexagonGrid grid;

  GridIndexer::OffsetGridIndexer indexer(&grid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = indexer.CreateCell("12020,1");
  std::list<FaceCoordinate> vertices;

  indexer.GetCellVertices(*cell, vertices);

  EXPECT_EQ(6U, vertices.size());

  const double tolerance = 1E-6;

  std::list<FaceCoordinate>::const_iterator vertex = vertices.begin();

  vertex = vertices.begin();
  EXPECT_NEAR(0.5, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(0.09622504, vertex->GetYOffset(), tolerance);

  ++vertex;
  EXPECT_NEAR(1.0 / 3.0, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(0.19245009, vertex->GetYOffset(), tolerance);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(0.09622504, vertex->GetYOffset(), tolerance);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(-0.09622504, vertex->GetYOffset(), tolerance);

  ++vertex;
  EXPECT_NEAR(1.0 / 3.0, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(-0.19245009, vertex->GetYOffset(), tolerance);

  ++vertex;
  EXPECT_NEAR(0.5, vertex->GetXOffset(), tolerance);
  EXPECT_NEAR(-0.09622504, vertex->GetYOffset(), tolerance);
}
