//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file TriangularFaceTest.cpp
/// 
/// Tests for the EAGGR::Model::TriangularFace class.
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

#include <cmath>

#include "Src/Model/TriangularFace.hpp"

using namespace EAGGR::Model;

static const double ACCURACY = 1E-20;

UNIT_TEST(TriangularFace, CalculateCellLocationVertices)
{
  TriangularFace face;

  FaceCoordinate onVertex1(0U, 0.0, sqrt(3.0) / 3.0, ACCURACY);
  EXPECT_EQ(Cell::VERTEX, face.CalculateCellLocation(onVertex1, ACCURACY));

  FaceCoordinate onVertex2(0U, -0.5, -sqrt(3.0) / 6.0, ACCURACY);
  EXPECT_EQ(Cell::VERTEX, face.CalculateCellLocation(onVertex2, ACCURACY));

  FaceCoordinate onVertex3(0U, 0.5, -sqrt(3.0) / 6.0, ACCURACY);
  EXPECT_EQ(Cell::VERTEX, face.CalculateCellLocation(onVertex3, ACCURACY));
}

UNIT_TEST(TriangularFace, CalculateCellLocationEdges)
{
  TriangularFace face;

  FaceCoordinate onLeftEdge(0U, -0.25, sqrt(3.0) / 12.0, ACCURACY);
  EXPECT_EQ(Cell::EDGE, face.CalculateCellLocation(onLeftEdge, ACCURACY));

  FaceCoordinate onRightEdge(0U, 0.25, sqrt(3.0) / 12.0, ACCURACY);
  EXPECT_EQ(Cell::EDGE, face.CalculateCellLocation(onLeftEdge, ACCURACY));

  FaceCoordinate onBottomEdge(0U, 0.0, -sqrt(3.0) / 6.0, ACCURACY);
  EXPECT_EQ(Cell::EDGE, face.CalculateCellLocation(onLeftEdge, ACCURACY));
}

UNIT_TEST(TriangularFace, CalculateCellLocationInsideFace)
{
  TriangularFace face;

  FaceCoordinate inPartition0(0U, 0.0, 0.0, ACCURACY);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(inPartition0, ACCURACY));

  FaceCoordinate inPartition1(0U, 0.0, sqrt(3.0) / 6.0, ACCURACY);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(inPartition1, ACCURACY));

  FaceCoordinate inPartition2(0U, -0.25, -sqrt(3.0) / 12.0, ACCURACY);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(inPartition2, ACCURACY));

  FaceCoordinate inPartition3(0U, 0.25, -sqrt(3.0) / 12.0, ACCURACY);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(inPartition3, ACCURACY));

  // Location covering whole face
  FaceCoordinate wholeFace(0U, 0.0, 0.0, 1.0);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(wholeFace, 1.0));
}

UNIT_TEST(TriangularFace, CalculateCellLocationTolerance)
{
  TriangularFace face;

  const double resolution3Accuracy = 2.0 / 27.0;

  // Cell at resolution 3 that is next to a cell on the vertex
  FaceCoordinate topCellNotOnVertex(0U, 0.0, 0.384900179, resolution3Accuracy);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(topCellNotOnVertex, ACCURACY));

  // Cell at resolution 3 that touches the edge but is wholly inside the face
  FaceCoordinate bottomCellNotOnEdge(0U, 0.0, -0.1924500897, resolution3Accuracy);
  EXPECT_EQ(Cell::FACE, face.CalculateCellLocation(bottomCellNotOnEdge, ACCURACY));
}
