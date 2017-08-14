//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file Aperture3HexagonGridTest.cpp
///
/// Tests the EAGGR::Model::Aperture3HexagonGrid class
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
#include <algorithm>

#include "TestMacros.hpp"

#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/FaceTypes.hpp"
#include "Src/Model/FaceCoordinate.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Grid;
using namespace EAGGR::Model::Grid::OffsetGrid;

static const double ROOT_3 = sqrt(3);
static const double TOLERANCE = 1E-6;
static const unsigned short MAX_FACE_INDEX = 19U;

// Define an offset to apply to the point location so that the point is moved just inside the hexagon
static const double POINT_OFFSET = 1E-6;

bool OffsetCoordinateSorter(OffsetCoordinate coordinate1, OffsetCoordinate coordinate2);

UNIT_TEST(Aperture3HexagonGrid, GetResolutionFromAccuracyTest)
{
  Aperture3HexagonGrid grid;

  // Test when the accuracy is larger than the face
  short resolution = grid.GetResolutionFromAccuracy(1.1);
  EXPECT_EQ(0, resolution);

  // Test when the accuracy is equal to the face size
  resolution = grid.GetResolutionFromAccuracy(1.0);
  EXPECT_EQ(0, resolution);

  // Resolutions decrease by factor of 3 from 2/3 of the face size
  // Test an accuracy close to the face size
  resolution = grid.GetResolutionFromAccuracy(0.9);
  EXPECT_EQ(0, resolution);

  resolution = grid.GetResolutionFromAccuracy(2.0 / 3.0);
  EXPECT_EQ(1, resolution);

  // Test accuracy exactly between resolutions 1 and 2
  resolution = grid.GetResolutionFromAccuracy(4.0 / 9.0);
  EXPECT_EQ(1, resolution);

  // Subtract a small amount to the above accuracy and it should be resoution 2
  resolution = grid.GetResolutionFromAccuracy((4.0 / 9.0) - 1E-10);
  EXPECT_EQ(2, resolution);

  resolution = grid.GetResolutionFromAccuracy(2.0 / (3.0 * 3.0));
  EXPECT_EQ(2, resolution);

  resolution = grid.GetResolutionFromAccuracy(2.0 / (3.0 * 3.0 * 3.0));
  EXPECT_EQ(3, resolution);

  resolution = grid.GetResolutionFromAccuracy(2.0 / (3.0 * 3.0 * 3.0 * 3.0));
  EXPECT_EQ(4, resolution);

  resolution = grid.GetResolutionFromAccuracy(2.0 / (3.0 * 3.0 * 3.0 * 3.0 * 3.0));
  EXPECT_EQ(5, resolution);

  EXPECT_THROW(grid.GetResolutionFromAccuracy(0.0), EAGGR::EAGGRException);
  EXPECT_THROW(grid.GetResolutionFromAccuracy(-0.5), EAGGR::EAGGRException);
}

UNIT_TEST(Aperture3HexagonGrid, GetAccuracyFromResolutionTest)
{
  Aperture3HexagonGrid grid;

  // Test for resolution 0
  double accuracy = grid.GetAccuracyFromResolution(0);
  EXPECT_NEAR(1.0, accuracy, TOLERANCE);

  // Test for resolution 1
  accuracy = grid.GetAccuracyFromResolution(1);
  EXPECT_NEAR(2.0 / 3.0, accuracy, TOLERANCE);

  // Test for resolution 2
  accuracy = grid.GetAccuracyFromResolution(2);
  EXPECT_NEAR(2.0 / (3.0 * 3.0), accuracy, TOLERANCE);

  // Test for resolution 3
  accuracy = grid.GetAccuracyFromResolution(3);
  EXPECT_NEAR(2.0 / (3.0 * 3.0 * 3.0), accuracy, TOLERANCE);

  // Test for resolution 4
  accuracy = grid.GetAccuracyFromResolution(4);
  EXPECT_NEAR(2.0 / (3.0 * 3.0 * 3.0 * 3.0), accuracy, TOLERANCE);

  // Test for resolution 5
  accuracy = grid.GetAccuracyFromResolution(5);
  EXPECT_NEAR(2.0 / (3.0 * 3.0 * 3.0 * 3.0 * 3.0), accuracy, TOLERANCE);
}

UNIT_TEST(Aperture3HexagonGrid, GetRowAndColumnAtOrigin)
{
  Aperture3HexagonGrid grid;

  long rowId;
  long columnId;

  // Point at face centre
  FaceCoordinate faceCoordinate1(10, 0.0, 0.0, 1.0);

  grid.GetRowAndColumn(1U, faceCoordinate1, rowId, columnId);

  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);

  // Point offset from centre but in the 0, 0 hexagon
  FaceCoordinate faceCoordinate2(10, 0.001, 0.001, 1.0);

  grid.GetRowAndColumn(1U, faceCoordinate2, rowId, columnId);

  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetRowAndColumnResolution0)
{
  Aperture3HexagonGrid grid;

  long rowId;
  long columnId;

  // Should be in row 0, column 0 no matter where on the face the point is
  FaceCoordinate faceCoordinate0(10, 0.0, 0.0, 1.0);
  FaceCoordinate faceCoordinate1(10, 0.5, 0.5, 1.0);
  FaceCoordinate faceCoordinate2(10, -0.2, -0.3, 1.0);

  grid.GetRowAndColumn(0U, faceCoordinate0, rowId, columnId);
  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);

  grid.GetRowAndColumn(0U, faceCoordinate1, rowId, columnId);
  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);

  grid.GetRowAndColumn(0U, faceCoordinate2, rowId, columnId);
  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetRowAndColumnResolution1)
{
  Aperture3HexagonGrid grid;

  long rowId;
  long columnId;

  // Point at face centre
  FaceCoordinate faceCoordinate1(10, 0.0, 0.0, 1.0);

  grid.GetRowAndColumn(1U, faceCoordinate1, rowId, columnId);

  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);

  // Point offset from centre but in the 0, 0 hexagon
  FaceCoordinate faceCoordinate2(10, 0.001, 0.001, 1.0);

  grid.GetRowAndColumn(1U, faceCoordinate2, rowId, columnId);

  EXPECT_EQ(0, rowId);
  EXPECT_EQ(0, columnId);

  /// @todo: Add tests here for points on face outside the top-level hexagon
}

UNIT_TEST(Aperture3HexagonGrid, GetRowAndColumnResolution2)
{
  Aperture3HexagonGrid grid;

  long rowId;
  long columnId;

  static const double HEXAGON_WIDTH = 1.0 / 3.0;
  static const unsigned short RESOLUTION = 2U;

  // Test points along x axis
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, HEXAGON_WIDTH * i, 0.0, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, columnId);
    EXPECT_EQ(0, rowId);
  }

  // Test points along horizontal line y = 0.14433756 (in the 'wings' of hexagon in row 0/1)
  // Should be placed in a different row depending on y coordinate
  bool evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, HEXAGON_WIDTH * i, 0.14433756, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), columnId);

    if (evenRow)
    {
      EXPECT_EQ(0, rowId);
    }
    else
    {
      EXPECT_EQ(1, rowId);
    }

    evenRow = !evenRow;
  }

  // Test points along horizontal line y = 0.28867514 (in row 1)
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, (HEXAGON_WIDTH * i) + POINT_OFFSET, 0.28867514, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, columnId);
    EXPECT_EQ(1, rowId);
  }

  // Test points along horizontal line y = 0.4330127 (in the 'wings' of hexagon in rows 1/2)
  // Should be placed in a different row depending on x coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, HEXAGON_WIDTH * i, 0.4330127, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), columnId);

    if (evenRow)
    {
      EXPECT_EQ(2, rowId);
    }
    else
    {
      EXPECT_EQ(1, rowId);
    }

    evenRow = !evenRow;
  }

  // Test points along horizontal line y = -0.14433757 (in the 'wings' of hexagon in rows -1/0)
  // Should be placed in a different row depending on x coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, HEXAGON_WIDTH * i, -0.14433757, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), columnId);

    if (evenRow)
    {
      EXPECT_EQ(0, rowId);
    }
    else
    {
      EXPECT_EQ(-1, rowId);
    }

    evenRow = !evenRow;
  }

  // Test points along horizontal line y = -0.28867514 (in row -1)
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, (HEXAGON_WIDTH * i) + POINT_OFFSET, -0.28867514, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, columnId);
    EXPECT_EQ(-1, rowId);
  }

  // Test points along horizontal line y = -0.43301271 (in the 'wings' of hexagon in rows 1/2)
  // Should be placed in a different row depending on x coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, HEXAGON_WIDTH * i, -0.43301271, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), columnId);

    if (evenRow)
    {
      EXPECT_EQ(-2, rowId);
    }
    else
    {
      EXPECT_EQ(-1, rowId);
    }

    evenRow = !evenRow;
  }
}

UNIT_TEST(Aperture3HexagonGrid, GetRowAndColumnResolution3)
{
  Aperture3HexagonGrid grid;

  long rowId;
  long columnId;

  static const double HEXAGON_HEIGHT = ROOT_3 / 9.0;
  static const unsigned short RESOLUTION = 3U;

  // Test points along y axis
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, 0.0, HEXAGON_HEIGHT * i, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, rowId);
    EXPECT_EQ(0, columnId);
  }

  // Test points along vertical line x = 0.0833333 (in the 'wings' of hexagon in columns 0/1)
  // Should be placed in a different column depending on y coordinate
  bool evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, 0.0833333, HEXAGON_HEIGHT * i, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), rowId);

    if (evenRow)
    {
      EXPECT_EQ(0, columnId);
    }
    else
    {
      EXPECT_EQ(1, columnId);
    }

    evenRow = !evenRow;
  }

  // Test points along vertical line x = 0.16667 (in column 1)
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, 0.16667, (HEXAGON_HEIGHT * i) + POINT_OFFSET, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, rowId);
    EXPECT_EQ(1, columnId);
  }

  // Test points along vertical line x = 0.25 (in the 'wings' of hexagon in columns 1/2)
  // Should be placed in a different column depending on y coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, 0.25, HEXAGON_HEIGHT * i, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), rowId);

    if (evenRow)
    {
      EXPECT_EQ(2, columnId);
    }
    else
    {
      EXPECT_EQ(1, columnId);
    }

    evenRow = !evenRow;
  }

  // Test points along vertical line x = -0.0833333 (in the 'wings' of hexagon in columns 0/1)
  // Should be placed in a different column depending on y coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, -0.0833333, HEXAGON_HEIGHT * i, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), rowId);

    if (evenRow)
    {
      EXPECT_EQ(0, columnId);
    }
    else
    {
      EXPECT_EQ(-1, columnId);
    }

    evenRow = !evenRow;
  }

  // Test points along vertical line x = -0.16667 (in column 1)
  for (double i = -5.0; i < 5.0; ++i)
  {
    FaceCoordinate faceCoordinate(10, -0.16667, (HEXAGON_HEIGHT * i) + POINT_OFFSET, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(i, rowId);
    EXPECT_EQ(-1, columnId);
  }

  // Test points along vertical line x = -0.25 (in the 'wings' of hexagon in columns 1/2)
  // Should be placed in a different column depending on y coordinate
  evenRow = true;
  for (double i = -5.0; i < 5.0; i += 0.5)
  {
    FaceCoordinate faceCoordinate(10, -0.25, HEXAGON_HEIGHT * i, 1.0);

    grid.GetRowAndColumn(RESOLUTION, faceCoordinate, rowId, columnId);

    EXPECT_EQ(floor(i), rowId);

    if (evenRow)
    {
      EXPECT_EQ(-2, columnId);
    }
    else
    {
      EXPECT_EQ(-1, columnId);
    }

    evenRow = !evenRow;
  }
}

UNIT_TEST(Aperture3HexagonGrid, GetParentsHorizontalCells)
{
  Aperture3HexagonGrid grid;

  // Horizontal cell resolution
  static const unsigned short RESOLUTION = 3;

  std::vector<OffsetCoordinate> parents;

  // Note that in cell variable names underscores are used to indicate negative coordinates

  // Check origin cell and six surrounding cells
  Cell::OffsetCell cell00(0U, RESOLUTION, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell00, parents);
  ASSERT_EQ(1U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);

  Cell::OffsetCell cell01(0U, RESOLUTION, 0, 1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell01, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(1, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell10(0U, RESOLUTION, 1, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell10, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
  EXPECT_EQ(1, parents.at(1).m_rowId);
  EXPECT_EQ(-1, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell0_1(0U, RESOLUTION, 0, -1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell0_1, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(-1, parents.at(2).m_columnId);

  Cell::OffsetCell cell_1_1(0U, RESOLUTION, -1, -1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_1_1, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(-1, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell_10(0U, RESOLUTION, -1, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_10, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(-1, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell_11(0U, RESOLUTION, -1, 1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_11, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(1, parents.at(2).m_columnId);

  // Check a non-origin cell with one parent and three parents
  Cell::OffsetCell cell_23(0U, RESOLUTION, -2, 3, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_23, parents);
  ASSERT_EQ(1U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(1, parents.at(0).m_columnId);

  Cell::OffsetCell cell1_2(0U, RESOLUTION, 1, -2, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell1_2, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(1, parents.at(1).m_rowId);
  EXPECT_EQ(-2, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(-1, parents.at(2).m_columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetParentsResolution1)
{
  Aperture3HexagonGrid grid;

  static const unsigned short RESOLUTION = 1;

  std::vector<OffsetCoordinate> parents;
  Cell::OffsetCell cellOrigin(0U, RESOLUTION, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cellOrigin, parents);

  // From resolution 1 the parent is the whole face
  EXPECT_EQ(1U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetParentsVerticalCells)
{
  Aperture3HexagonGrid grid;

  // Vertical cell resolution
  static const unsigned short RESOLUTION = 2;

  std::vector<OffsetCoordinate> parents;

  // Note that in cell variable names underscores are used to indicate negative coordinates

  // Check origin cell and six surrounding cells
  Cell::OffsetCell cell00(0U, RESOLUTION, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell00, parents);
  ASSERT_EQ(1U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);

  Cell::OffsetCell cell01(0U, RESOLUTION, 0, 1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell01, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(1, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(1, parents.at(2).m_columnId);

  Cell::OffsetCell cell10(0U, RESOLUTION, 1, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell10, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(1, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell1_1(0U, RESOLUTION, 1, -1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell1_1, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell0_1(0U, RESOLUTION, 0, -1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell0_1, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(-1, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell_1_1(0U, RESOLUTION, -1, -1, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_1_1, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(-1, parents.at(0).m_columnId);
  EXPECT_EQ(-1, parents.at(1).m_rowId);
  EXPECT_EQ(0, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  Cell::OffsetCell cell_10(0U, RESOLUTION, -1, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_10, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(0, parents.at(0).m_columnId);
  EXPECT_EQ(-1, parents.at(1).m_rowId);
  EXPECT_EQ(1, parents.at(1).m_columnId);
  EXPECT_EQ(0, parents.at(2).m_rowId);
  EXPECT_EQ(0, parents.at(2).m_columnId);

  // Check a non-origin cell with one parent and three parents
  Cell::OffsetCell cell_23(0U, RESOLUTION, -2, 3, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell_23, parents);
  ASSERT_EQ(1U, parents.size());
  EXPECT_EQ(-1, parents.at(0).m_rowId);
  EXPECT_EQ(2, parents.at(0).m_columnId);

  Cell::OffsetCell cell1_3(0U, RESOLUTION, 1, -3, Cell::FACE, MAX_FACE_INDEX);
  grid.GetParents(cell1_3, parents);
  std::sort(parents.begin(), parents.end(), OffsetCoordinateSorter);
  ASSERT_EQ(3U, parents.size());
  EXPECT_EQ(0, parents.at(0).m_rowId);
  EXPECT_EQ(-2, parents.at(0).m_columnId);
  EXPECT_EQ(0, parents.at(1).m_rowId);
  EXPECT_EQ(-1, parents.at(1).m_columnId);
  EXPECT_EQ(1, parents.at(2).m_rowId);
  EXPECT_EQ(-2, parents.at(2).m_columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetChildrenHorizontalCells)
{
  Aperture3HexagonGrid grid;

  // Horizontal cell resolution
  static const unsigned short RESOLUTION = 1;

  // Origin
  std::vector<OffsetCoordinate> children;
  Cell::OffsetCell cellOrigin(0U, RESOLUTION, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellOrigin, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(-1, children.at(0).m_rowId);
  EXPECT_EQ(-1, children.at(0).m_columnId);
  EXPECT_EQ(-1, children.at(1).m_rowId);
  EXPECT_EQ(0, children.at(1).m_columnId);
  EXPECT_EQ(0, children.at(2).m_rowId);
  EXPECT_EQ(-1, children.at(2).m_columnId);
  EXPECT_EQ(0, children.at(3).m_rowId);
  EXPECT_EQ(0, children.at(3).m_columnId);
  EXPECT_EQ(0, children.at(4).m_rowId);
  EXPECT_EQ(1, children.at(4).m_columnId);
  EXPECT_EQ(1, children.at(5).m_rowId);
  EXPECT_EQ(-1, children.at(5).m_columnId);
  EXPECT_EQ(1, children.at(6).m_rowId);
  EXPECT_EQ(0, children.at(6).m_columnId);

  // Even column
  Cell::OffsetCell cellEvenColumn(0U, RESOLUTION, 1, 2, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellEvenColumn, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(1, children.at(0).m_rowId);
  EXPECT_EQ(2, children.at(0).m_columnId);
  EXPECT_EQ(1, children.at(1).m_rowId);
  EXPECT_EQ(3, children.at(1).m_columnId);
  EXPECT_EQ(2, children.at(2).m_rowId);
  EXPECT_EQ(2, children.at(2).m_columnId);
  EXPECT_EQ(2, children.at(3).m_rowId);
  EXPECT_EQ(3, children.at(3).m_columnId);
  EXPECT_EQ(2, children.at(4).m_rowId);
  EXPECT_EQ(4, children.at(4).m_columnId);
  EXPECT_EQ(3, children.at(5).m_rowId);
  EXPECT_EQ(2, children.at(5).m_columnId);
  EXPECT_EQ(3, children.at(6).m_rowId);
  EXPECT_EQ(3, children.at(6).m_columnId);

  // Odd column
  Cell::OffsetCell cellOddColumn(0U, RESOLUTION, 2, 3, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellOddColumn, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(4, children.at(0).m_rowId);
  EXPECT_EQ(4, children.at(0).m_columnId);
  EXPECT_EQ(4, children.at(1).m_rowId);
  EXPECT_EQ(5, children.at(1).m_columnId);
  EXPECT_EQ(5, children.at(2).m_rowId);
  EXPECT_EQ(3, children.at(2).m_columnId);
  EXPECT_EQ(5, children.at(3).m_rowId);
  EXPECT_EQ(4, children.at(3).m_columnId);
  EXPECT_EQ(5, children.at(4).m_rowId);
  EXPECT_EQ(5, children.at(4).m_columnId);
  EXPECT_EQ(6, children.at(5).m_rowId);
  EXPECT_EQ(4, children.at(5).m_columnId);
  EXPECT_EQ(6, children.at(6).m_rowId);
  EXPECT_EQ(5, children.at(6).m_columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetChildrenVerticalCells)
{
  Aperture3HexagonGrid grid;

  // Vertical cell resolution
  static const unsigned short RESOLUTION = 2;

  // Origin
  std::vector<OffsetCoordinate> children;
  Cell::OffsetCell cellOrigin(0U, RESOLUTION, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellOrigin, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(-1, children.at(0).m_rowId);
  EXPECT_EQ(-1, children.at(0).m_columnId);
  EXPECT_EQ(-1, children.at(1).m_rowId);
  EXPECT_EQ(0, children.at(1).m_columnId);
  EXPECT_EQ(-1, children.at(2).m_rowId);
  EXPECT_EQ(1, children.at(2).m_columnId);
  EXPECT_EQ(0, children.at(3).m_rowId);
  EXPECT_EQ(-1, children.at(3).m_columnId);
  EXPECT_EQ(0, children.at(4).m_rowId);
  EXPECT_EQ(0, children.at(4).m_columnId);
  EXPECT_EQ(0, children.at(5).m_rowId);
  EXPECT_EQ(1, children.at(5).m_columnId);
  EXPECT_EQ(1, children.at(6).m_rowId);
  EXPECT_EQ(0, children.at(6).m_columnId);

  // Even row
  Cell::OffsetCell cellEvenRow(0U, RESOLUTION, 2, 3, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellEvenRow, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(2, children.at(0).m_rowId);
  EXPECT_EQ(5, children.at(0).m_columnId);
  EXPECT_EQ(2, children.at(1).m_rowId);
  EXPECT_EQ(6, children.at(1).m_columnId);
  EXPECT_EQ(2, children.at(2).m_rowId);
  EXPECT_EQ(7, children.at(2).m_columnId);
  EXPECT_EQ(3, children.at(3).m_rowId);
  EXPECT_EQ(5, children.at(3).m_columnId);
  EXPECT_EQ(3, children.at(4).m_rowId);
  EXPECT_EQ(6, children.at(4).m_columnId);
  EXPECT_EQ(3, children.at(5).m_rowId);
  EXPECT_EQ(7, children.at(5).m_columnId);
  EXPECT_EQ(4, children.at(6).m_rowId);
  EXPECT_EQ(6, children.at(6).m_columnId);

  // Odd row
  Cell::OffsetCell cellOddRow(0U, RESOLUTION, 3, 2, Cell::FACE, MAX_FACE_INDEX);
  grid.GetChildren(cellOddRow, children);
  std::sort(children.begin(), children.end(), OffsetCoordinateSorter);
  EXPECT_EQ(7U, children.size());
  EXPECT_EQ(3, children.at(0).m_rowId);
  EXPECT_EQ(5, children.at(0).m_columnId);
  EXPECT_EQ(4, children.at(1).m_rowId);
  EXPECT_EQ(4, children.at(1).m_columnId);
  EXPECT_EQ(4, children.at(2).m_rowId);
  EXPECT_EQ(5, children.at(2).m_columnId);
  EXPECT_EQ(4, children.at(3).m_rowId);
  EXPECT_EQ(6, children.at(3).m_columnId);
  EXPECT_EQ(5, children.at(4).m_rowId);
  EXPECT_EQ(4, children.at(4).m_columnId);
  EXPECT_EQ(5, children.at(5).m_rowId);
  EXPECT_EQ(5, children.at(5).m_columnId);
  EXPECT_EQ(5, children.at(6).m_rowId);
  EXPECT_EQ(6, children.at(6).m_columnId);
}

UNIT_TEST(Aperture3HexagonGrid, GetVertices)
{
  Aperture3HexagonGrid grid;

  // Resolution 0
  Cell::OffsetCell cellResolution0(1U, 0U, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  std::list<FaceCoordinate> verticesResolution0;
  grid.GetVertices(cellResolution0, verticesResolution0);
  EXPECT_EQ(3U, verticesResolution0.size());

  std::list<FaceCoordinate>::const_iterator vertex = verticesResolution0.begin();
  EXPECT_EQ(0.0, vertex->GetXOffset());
  EXPECT_EQ(sqrt(3.0) / 3.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(-0.5, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(0.5, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());

  // Resolution 1
  Cell::OffsetCell cellResolution1(1U, 1U, 0, 0, Cell::FACE, MAX_FACE_INDEX);
  std::list<FaceCoordinate> verticesResolution1;
  grid.GetVertices(cellResolution1, verticesResolution1);
  EXPECT_EQ(6U, verticesResolution1.size());

  vertex = verticesResolution1.begin();
  EXPECT_NEAR(1.0 / 3.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.0, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.28867513, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(-1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.28867513, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(-1.0 / 3.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.0, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(-1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(-0.28867513, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(-0.28867513, vertex->GetYOffset(), TOLERANCE);

  // Resolution 2
  Cell::OffsetCell cellResolution2(1U, 2U, 0, 1, Cell::FACE, MAX_FACE_INDEX);
  std::list<FaceCoordinate> verticesResolution2;
  grid.GetVertices(cellResolution2, verticesResolution2);
  EXPECT_EQ(6U, verticesResolution2.size());

  vertex = verticesResolution2.begin();
  EXPECT_NEAR(0.5, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.09622504, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 3.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.19245009, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(0.09622504, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 6.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(-0.09622504, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(1.0 / 3.0, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(-0.19245009, vertex->GetYOffset(), TOLERANCE);

  ++vertex;
  EXPECT_NEAR(0.5, vertex->GetXOffset(), TOLERANCE);
  EXPECT_NEAR(-0.09622504, vertex->GetYOffset(), TOLERANCE);
}

UNIT_TEST(Aperture3HexagonGrid, GetNumberOfChildren)
{
  Aperture3HexagonGrid grid;

  EXPECT_EQ(7U, grid.GetNumChildren());
}

UNIT_TEST(Aperture3HexagonGrid, GetAperture)
{
  Aperture3HexagonGrid grid;

  EXPECT_EQ(3U, grid.GetAperture());
}

bool OffsetCoordinateSorter(OffsetCoordinate coordinate1, OffsetCoordinate coordinate2)
{
  // Sort on row id first
  if (coordinate1.m_rowId < coordinate2.m_rowId)
  {
    return true;
  }
  else if (coordinate1.m_rowId > coordinate2.m_rowId)
  {
    return false;
  }

  // If the row ids are equal then sort on column id
  if (coordinate1.m_columnId < coordinate2.m_columnId)
  {
    return true;
  }
  else
  {
    return false;
  }
}
