//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file Aperture4TriangleGridTest.cpp
///
/// Tests the EAGGR::Model::Grid::HierarchicalGrid::Aperture4TriangleGrid class
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

#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/FaceTypes.hpp"
#include "Src/Model/IGrid/CellPartition.hpp"
#include "Src/Model/ICell/HierarchicalCell.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Grid;
using namespace EAGGR::Model::Grid::HierarchicalGrid;

static const double ROOT_3 = sqrt(3);
static const double TOLERANCE = 1E-6;
static const unsigned short MAX_FACE_INDEX = 19U;
static const unsigned short MAX_CELL_INDEX = 3U;

UNIT_TEST(Aperture4TriangleGrid, GetResolutionFromAccuracy)
{
  Aperture4TriangleGrid grid;

  // Test when the accuracy is larger than the face
  short resolution = grid.GetResolutionFromAccuracy(1.1);
  EXPECT_EQ(0, resolution);

  // Test when the accuracy is equal to the face size
  resolution = grid.GetResolutionFromAccuracy(1.0);
  EXPECT_EQ(0, resolution);

  // Resolutions decrease by factor of 4
  // Test an accuracy close to the face size
  resolution = grid.GetResolutionFromAccuracy(0.75);
  EXPECT_EQ(0, resolution);

  // Just above the mid-point between resolutions 0 and 1
  resolution = grid.GetResolutionFromAccuracy(1.25 / 2.0 + 0.00001);
  EXPECT_EQ(0, resolution);

  // just below the mid-point between resolutions 0 and 1
  resolution = grid.GetResolutionFromAccuracy(1.25 / 2.0 - 0.00001);
  EXPECT_EQ(1, resolution);

  resolution = grid.GetResolutionFromAccuracy(0.3);
  EXPECT_EQ(1, resolution);

  // Test at points at the sizes of the resolutions
  resolution = grid.GetResolutionFromAccuracy(1.0 / 4.0);
  EXPECT_EQ(1, resolution);

  resolution = grid.GetResolutionFromAccuracy(1.0 / (4.0 * 4.0));
  EXPECT_EQ(2, resolution);

  resolution = grid.GetResolutionFromAccuracy(1.0 / (4.0 * 4.0 * 4.0));
  EXPECT_EQ(3, resolution);

  resolution = grid.GetResolutionFromAccuracy(1.0 / (4.0 * 4.0 * 4.0 * 4.0));
  EXPECT_EQ(4, resolution);

  resolution = grid.GetResolutionFromAccuracy(1.0 / (4.0 * 4.0 * 4.0 * 4.0 * 4.0));
  EXPECT_EQ(5, resolution);

  EXPECT_THROW(grid.GetResolutionFromAccuracy(0.0), EAGGR::EAGGRException);
  EXPECT_THROW(grid.GetResolutionFromAccuracy(-0.5), EAGGR::EAGGRException);
}

UNIT_TEST(Aperture4TriangleGrid, GetAccuracyFromResolution)
{
  Aperture4TriangleGrid grid;

  // Test for resolution 0
  double accuracy = grid.GetAccuracyFromResolution(0);
  EXPECT_EQ(1.0, accuracy);

  // Test for resolution 1
  accuracy = grid.GetAccuracyFromResolution(1);
  EXPECT_EQ(1.0 / 4.0, accuracy);

  // Test for resolution 2
  accuracy = grid.GetAccuracyFromResolution(2);
  EXPECT_EQ(1.0 / (4.0 * 4.0), accuracy);

  // Test for resolution 3
  accuracy = grid.GetAccuracyFromResolution(3);
  EXPECT_EQ(1.0 / (4.0 * 4.0 * 4.0), accuracy);

  // Test for resolution 4
  accuracy = grid.GetAccuracyFromResolution(4);
  EXPECT_EQ(1.0 / (4.0 * 4.0 * 4.0 * 4.0), accuracy);

  // Test for resolution 5
  accuracy = grid.GetAccuracyFromResolution(5);
  EXPECT_EQ(1.0 / (4.0 * 4.0 * 4.0 * 4.0 * 4.0), accuracy);
}

UNIT_TEST(Aperture4TriangleGrid, GetFacePartitionFaceCentre)
{
  Aperture4TriangleGrid grid;

  FaceCoordinate faceCentre(0, 0.0, 0.0, 1.0);

  unsigned short partitionId = 9; // initialise to invalid value
  ShapeOrientation partitionOrientation = STANDARD;
  CartesianPoint partitionCentre(0.0, 0.0);
  CellPartition partition(partitionId, partitionCentre, partitionOrientation);

  grid.GetFacePartition(
      partition,
      1,
      faceCentre,
      &partition);
  EXPECT_EQ(0, partition.GetId());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetY());
}

UNIT_TEST(Aperture4TriangleGrid, GetFacePartitionTopPartition)
{
  Aperture4TriangleGrid grid;

  FaceCoordinate pointInTopPartition(0, 0.1, 0.3, 1.0);

  unsigned short partitionId = 9; // initialise to invalid value
  ShapeOrientation partitionOrientation = STANDARD;
  CartesianPoint partitionCentre(0.0, 0.0);
  CellPartition partition(partitionId, partitionCentre, partitionOrientation);

  grid.GetFacePartition(
      partition,
      1,
      pointInTopPartition,
      &partition);
  EXPECT_EQ(1, partition.GetId());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(sqrt(3.0) / 6.0, partition.GetPartitionCentre().GetY());
}

UNIT_TEST(Aperture4TriangleGrid, GetFacePartitionLeftPartition)
{
  Aperture4TriangleGrid grid;

  FaceCoordinate pointInLeftPartition(0, -0.2, 0.0, 1.0);

  unsigned short partitionId = 9; // initialise to invalid value
  ShapeOrientation partitionOrientation = STANDARD;
  CartesianPoint partitionCentre(0.0, 0.0);
  CellPartition partition(partitionId, partitionCentre, partitionOrientation);

  grid.GetFacePartition(
      partition,
      1,
      pointInLeftPartition,
      &partition);
  EXPECT_EQ(2, partition.GetId());
  EXPECT_DOUBLE_EQ(-0.25, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(-sqrt(3.0) / 12.0, partition.GetPartitionCentre().GetY());
}

UNIT_TEST(Aperture4TriangleGrid, GetFacePartitionRightPartition)
{
  Aperture4TriangleGrid grid;

  FaceCoordinate pointInRightPartition(0, 0.2, 0.0, 1.0);

  unsigned short partitionId = 9; // initialise to invalid value
  ShapeOrientation partitionOrientation = STANDARD;
  CartesianPoint partitionCentre(0.0, 0.0);
  CellPartition partition(partitionId, partitionCentre, partitionOrientation);

  grid.GetFacePartition(
      partition,
      1,
      pointInRightPartition,
      &partition);
  EXPECT_EQ(3, partition.GetId());
  EXPECT_DOUBLE_EQ(0.25, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(-sqrt(3.0) / 12.0, partition.GetPartitionCentre().GetY());
}

UNIT_TEST(Aperture4TriangleGrid, CornerPointsResolution1Partition0)
{
  Aperture4TriangleGrid grid;

  // The vertices of the central partition of the triangle
  FaceCoordinate leftVertex(0, -0.125, -sqrt(3.0) / 24.0, 1.0);
  FaceCoordinate rightVertex(0, 0.125, -sqrt(3.0) / 24.0, 1.0);
  FaceCoordinate topVertex(0, 0.0, sqrt(3.0) / 12.0, 1.0);

  unsigned short partitionId = 9;// initialise to invalid value
  ShapeOrientation partitionOrientation = STANDARD;
  CartesianPoint partitionCentre(0.0, 0.0);
  CellPartition partition(partitionId, partitionCentre, partitionOrientation);

  grid.GetFacePartition(
      partition,
      1,
      leftVertex,
      &partition);
  EXPECT_EQ(0, partition.GetId());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetY());

  grid.GetFacePartition(
      partition,
      1,
      rightVertex,
      &partition);
  EXPECT_EQ(0, partition.GetId());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetY());

  grid.GetFacePartition(
      partition,
      1,
      topVertex,
      &partition);
  EXPECT_EQ(0, partition.GetId());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetX());
  EXPECT_DOUBLE_EQ(0.0, partition.GetPartitionCentre().GetY());
}

UNIT_TEST(Aperture4TriangleGrid, AllCornerPointsResolution3)
{
  double delta = 1E-10;
  double triangleHeight = ROOT_3 / 16.0;
  double triangleWidth = 0.125;
  double triangleTop = ROOT_3 / 3.0;
  short partitionValues[4] =
  { 1, 2, 0, 3};

  double pointXValues[192];
  double pointYValues[192];

  int expectedPartition[192];
  double expectedCellCentreX[192];
  double expectedCellCentreY[192];

  int pointId = 0;
  int triangleId = 0;

  int trianglesPerRow = 1;

  for (int triangleRow = 0; triangleRow < 8; ++triangleRow)
  {
    for (int triangle = -trianglesPerRow / 2; triangle <= trianglesPerRow / 2; ++triangle)
    {
      double triangleCentreX = triangle * triangleWidth * 0.5;

      bool isInverted = (triangleRow % 2 == 0 && triangle % 2 != 0)
      || (triangleRow % 2 != 0 && triangle % 2 == 0);
      int yOffsetSign = isInverted ? -1 : 1;

      double triangleCentreYOffset = isInverted ? triangleHeight / 3.0 : triangleHeight * 2.0 / 3.0;

      double triangleCentreY = triangleTop - (triangleRow * triangleHeight) - triangleCentreYOffset;

      pointXValues[pointId] = triangleCentreX;
      pointYValues[pointId] = triangleCentreY
      + (yOffsetSign * ((triangleHeight * 2.0 / 3.0) - delta));

      pointXValues[pointId + 1] = triangleCentreX - (triangleWidth / 2.0) + delta;
      pointYValues[pointId + 1] = triangleCentreY
      + (yOffsetSign * ((-triangleHeight / 3.0) + delta));

      pointXValues[pointId + 2] = triangleCentreX + (triangleWidth / 2.0) - delta;
      pointYValues[pointId + 2] = triangleCentreY
      + (yOffsetSign * ((-triangleHeight / 3.0) + delta));

      expectedPartition[pointId] = partitionValues[triangleId % 4];
      expectedPartition[pointId + 1] = partitionValues[triangleId % 4];
      expectedPartition[pointId + 2] = partitionValues[triangleId % 4];

      expectedCellCentreX[pointId] = triangleCentreX;
      expectedCellCentreX[pointId + 1] = triangleCentreX;
      expectedCellCentreX[pointId + 2] = triangleCentreX;
      expectedCellCentreY[pointId] = triangleCentreY;
      expectedCellCentreY[pointId + 1] = triangleCentreY;
      expectedCellCentreY[pointId + 2] = triangleCentreY;

      pointId += 3;
      ++triangleId;
    }

    trianglesPerRow += 2;
  }

  Aperture4TriangleGrid grid;

  for (int i = 0; i < 192; ++i)
  {
    FaceCoordinate point(0, pointXValues[i], pointYValues[i], 1.0);

    unsigned short partitionId = 9; // initialise to invalid value
    ShapeOrientation partitionOrientation = STANDARD;
    CartesianPoint partitionCentre(0.0, 0.0);
    CellPartition partition(partitionId, partitionCentre, partitionOrientation);

    grid.GetFacePartition(
        partition,
        1,
        point,
        &partition);
    grid.GetFacePartition(
        partition,
        2,
        point,
        &partition);
    grid.GetFacePartition(
        partition,
        3,
        point,
        &partition);
    EXPECT_EQ(expectedPartition[i], partition.GetId());
    EXPECT_NEAR(expectedCellCentreX[i], partition.GetPartitionCentre().GetX(), TOLERANCE);
    EXPECT_NEAR(expectedCellCentreY[i], partition.GetPartitionCentre().GetY(), TOLERANCE);
  }
}

UNIT_TEST(Aperture4TriangleGrid, GetFaceCoordinateResolution1)
{
  Aperture4TriangleGrid grid;

  double xOffset;
  double yOffset;

  unsigned short indices[4][1] =
  {
    { 0},
    { 1},
    { 2},
    { 3}};

  double expectedXOffset[4] =
  { 0.0, 0.0, -0.25, 0.25};
  double expectedYOffset[4] =
  { 0.0, ROOT_3 / 6.0, -ROOT_3 / 12.0, -ROOT_3 / 12.0};

  for (int index = 0; index < 4; ++index)
  {
    std::vector<unsigned short> cellIndices;
    cellIndices.push_back(indices[index][0]);
    Cell::HierarchicalCell cell(0, cellIndices, MAX_FACE_INDEX, grid.GetAperture() - 1U);

    grid.GetFaceOffset(cell, xOffset, yOffset);

    EXPECT_NEAR(expectedXOffset[index], xOffset, TOLERANCE);
    EXPECT_NEAR(expectedYOffset[index], yOffset, TOLERANCE);
  }
}

UNIT_TEST(Aperture4TriangleGrid, GetFaceCoordinateResolution2)
{
  Aperture4TriangleGrid grid;

  double xOffset;
  double yOffset;

  unsigned short indices[16][2] =
  {
    { 0, 0},
    { 0, 1},
    { 0, 2},
    { 0, 3},
    { 1, 0},
    { 1, 1},
    { 1, 2},
    { 1, 3},
    { 2, 0},
    { 2, 1},
    { 2, 2},
    { 2, 3},
    { 3, 0},
    { 3, 1},
    { 3, 2},
    { 3, 3}};

  // Values calculated using the geometry of the triangular grid
  double expectedXOffset[16] =
  { 0.0, 0.0, -0.125, 0.125, 0.0, 0.0, -0.125, 0.125, -0.25, -0.25, -0.375, -0.125, 0.25, 0.25,
    0.125, 0.375};
  double expectedYOffset[16] =
  { 0.0, -ROOT_3 / 12.0, ROOT_3 / 24.0, ROOT_3 / 24.0, ROOT_3 / 6.0, ROOT_3 / 4.0, ROOT_3 / 8.0,
    ROOT_3 / 8.0, -ROOT_3 / 12.0, 0.0, -ROOT_3 / 8.0, -ROOT_3 / 8.0, -ROOT_3 / 12.0, 0.0, -ROOT_3
    / 8.0, -ROOT_3 / 8.0};

  for (int index = 0; index < 16; ++index)
  {
    std::vector<unsigned short> cellIndices;
    cellIndices.push_back(indices[index][0]);
    cellIndices.push_back(indices[index][1]);
    Cell::HierarchicalCell cell(5, cellIndices, MAX_FACE_INDEX, grid.GetAperture() - 1U);

    grid.GetFaceOffset(cell, xOffset, yOffset);

    EXPECT_NEAR(expectedXOffset[index], xOffset, TOLERANCE);
    EXPECT_NEAR(expectedYOffset[index], yOffset, TOLERANCE);
  }
}

UNIT_TEST(Aperture4TriangleGrid, ConvertToAndFromDGGSCellResolution3)
{
  double triangleHeight = ROOT_3 / 16.0;
  double triangleWidth = 0.125;
  double triangleTop = ROOT_3 / 3.0;

  // There are 64 triangles at this resolution
  double pointXValues[64];
  double pointYValues[64];

  // Index into the X and Y values arrays above
  int pointId = 0;

  // Keep track of the number of triangles in the current row
  int trianglesPerRow = 1;

  // This resolution has 8 rows of triangles in the grid
  for (int triangleRow = 0; triangleRow < 8; ++triangleRow)
  {
    // Loop over each triangle in the row
    // Half the triangles are in the negative x half of the triangle
    for (int triangle = -trianglesPerRow / 2; triangle <= trianglesPerRow / 2; ++triangle)
    {
      // Calculate x coordinate of the centre of this triangle
      double triangleCentreX = triangle * triangleWidth * 0.5;

      // On even rows, odd indexed triangles are inverted
      // On odd rows, even indexed triangles are inverted
      bool isInverted = (triangleRow % 2 == 0 && triangle % 2 != 0)
      || (triangleRow % 2 != 0 && triangle % 2 == 0);

      // Inverted triangles have centre offset 1/3 or triangle height; non-inverted 2/3
      double triangleCentreYOffset = isInverted ? triangleHeight / 3.0 : triangleHeight * 2.0 / 3.0;

      // Calculate the y coordinate of the centre of this triangle (offset from the top of the triangle face)
      double triangleCentreY = triangleTop - (triangleRow * triangleHeight) - triangleCentreYOffset;

      pointXValues[pointId] = triangleCentreX;
      pointYValues[pointId] = triangleCentreY;

      ++pointId;
    }

    trianglesPerRow += 2;
  }

  Aperture4TriangleGrid grid;

  for (int i = 0; i < 64; ++i)
  {
    FaceCoordinate point(0, pointXValues[i], pointYValues[i], 1.0);

    unsigned short partitionId = 9; // initialise to invalid value
    ShapeOrientation partitionOrientation = STANDARD;
    CartesianPoint partitionCentre(0.0, 0.0);
    CellPartition partition(partitionId, partitionCentre, partitionOrientation);

    std::vector<unsigned short> cellIndices;

    // Populate the indices at each resolution level
    grid.GetFacePartition(
        partition,
        1,
        point,
        &partition);

    cellIndices.push_back(partition.GetId());

    grid.GetFacePartition(
        partition,
        2,
        point,
        &partition);

    cellIndices.push_back(partition.GetId());

    grid.GetFacePartition(
        partition,
        3,
        point,
        &partition);

    cellIndices.push_back(partition.GetId());

    Cell::HierarchicalCell cell(0U, cellIndices, MAX_FACE_INDEX, grid.GetAperture() - 1U);

    // Convert the indices back to an x, y offset
    double xOffset;
    double yOffset;
    grid.GetFaceOffset(cell, xOffset, yOffset);

    EXPECT_NEAR(pointXValues[i], xOffset, TOLERANCE);
    EXPECT_NEAR(pointYValues[i], yOffset, TOLERANCE);
  }
}

UNIT_TEST(Aperture4TriangleGrid, GetNumberOfChildren)
{
  Aperture4TriangleGrid grid;

  EXPECT_EQ(4U, grid.GetNumChildren());
}

UNIT_TEST(Aperture4TriangleGrid, GetAperture)
{
  Aperture4TriangleGrid grid;

  EXPECT_EQ(4U, grid.GetAperture());
}

UNIT_TEST(Aperture4TriangleGrid, GetOrientation)
{
  Aperture4TriangleGrid grid;

  // Resolution 0
  Cell::HierarchicalCell cellResolution0("00", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(STANDARD, grid.GetOrientation(cellResolution0));

  // Resolution 1
  Cell::HierarchicalCell cellResolution1Partition0("000", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(ROTATED, grid.GetOrientation(cellResolution1Partition0));

  Cell::HierarchicalCell cellResolution1Partition1("001", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(STANDARD, grid.GetOrientation(cellResolution1Partition1));

  Cell::HierarchicalCell cellResolution1Partition2("002", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(STANDARD, grid.GetOrientation(cellResolution1Partition2));

  Cell::HierarchicalCell cellResolution1Partition3("003", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(STANDARD, grid.GetOrientation(cellResolution1Partition3));

  // Resolution 2
  Cell::HierarchicalCell cellResolution2Partition0("0000", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(STANDARD, grid.GetOrientation(cellResolution2Partition0));

  Cell::HierarchicalCell cellResolution2Partition1("0001", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(ROTATED, grid.GetOrientation(cellResolution2Partition1));

  Cell::HierarchicalCell cellResolution2Partition2("0002", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(ROTATED, grid.GetOrientation(cellResolution2Partition2));

  Cell::HierarchicalCell cellResolution2Partition3("0003", MAX_FACE_INDEX, MAX_CELL_INDEX);
  EXPECT_EQ(ROTATED, grid.GetOrientation(cellResolution2Partition3));
}

UNIT_TEST(Aperture4TriangleGrid, GetVertices)
{
  Aperture4TriangleGrid grid;

  // Resolution 0
  Cell::HierarchicalCell cellResolution0("00", MAX_FACE_INDEX, MAX_CELL_INDEX);
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
  Cell::HierarchicalCell cellResolution1("003", MAX_FACE_INDEX, MAX_CELL_INDEX);
  std::list<FaceCoordinate> verticesResolution1;
  grid.GetVertices(cellResolution1, verticesResolution1);
  EXPECT_EQ(3U, verticesResolution1.size());

  vertex = verticesResolution1.begin();
  EXPECT_EQ(0.25, vertex->GetXOffset());
  EXPECT_EQ(sqrt(3.0) / 12.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(0.0, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());

  ++vertex;
  EXPECT_EQ(0.5, vertex->GetXOffset());
  EXPECT_EQ(-sqrt(3.0) / 6.0, vertex->GetYOffset());
}
