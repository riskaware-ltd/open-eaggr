//------------------------------------------------------
// Copyright (c) Riskaware 2017
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file OGCTest.cpp
/// 
/// Tests to demonstrate compliance with the OGC standard where this is not proved elsewhere
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
#include "Src/Model/IProjection/Snyder.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"

static const double SURFACE_AREA_OF_EARTH = 510072000000000;
static const double RADIUS_OF_EARTH = 6371000;

static const unsigned int MAX_FACE_INDEX = 12U;
static const double TOLERANCE = 1E-8;

using namespace EAGGR::Model;

double GetPolygonArea(std::list<FaceCoordinate> vertices)
{
  // Set up two iterators to point to the first two vertices of the polygon
  std::list<FaceCoordinate>::const_iterator vertexIter1 = vertices.begin();
  std::list<FaceCoordinate>::const_iterator vertexIter2 = vertices.begin();
  ++vertexIter2;

  // The algorithm described here - http://www.mathopenref.com/coordpolygonarea.html -
  // calculates double area by iterating over the vertices and halves it at the end
  double totalDoubleArea(0.0);

  while (vertexIter1 != vertices.end())
  {
    FaceCoordinate vertex1 = *vertexIter1;
    FaceCoordinate vertex2 = *vertexIter2;

    totalDoubleArea += (vertex1.GetXOffset() * vertex2.GetYOffset())
        - (vertex1.GetYOffset() * vertex2.GetXOffset());

    ++vertexIter1;
    ++vertexIter2;

    // Second vertex needs to wrap so that the final term uses the last vertex and the first vertex
    if (vertexIter2 == vertices.end())
    {
      vertexIter2 = vertices.begin();
    }
  }

  return 0.5 * fabs(totalDoubleArea);
}

CartesianPoint GetPolygonCentroid(std::list<FaceCoordinate> vertices)
{
  double totalX(0.0);
  double totalY(0.0);

  for (std::list<FaceCoordinate>::const_iterator iter = vertices.begin(); iter != vertices.end();
      ++iter)
  {
    totalX += (*iter).GetXOffset();
    totalY += (*iter).GetYOffset();
  }

  return CartesianPoint(totalX / vertices.size(), totalY / vertices.size());
}

// This test proves requirement A.1.3
UNIT_TEST(OGC, CellBoundaryOverlapTriangle)
{
  // Find the vertex coordinates in lat/long of cells that touch a central cell
  // Each surrounding cell should have two coordinates in common with the central cell

  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;
  GridIndexer::HierarchicalGridIndexer hierarchicalGridIndexer(&triangleGrid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> centralCell = hierarchicalGridIndexer.CreateCell("0000000");
  std::unique_ptr<Cell::ICell> cell1 = hierarchicalGridIndexer.CreateCell("0000001");
  std::unique_ptr<Cell::ICell> cell2 = hierarchicalGridIndexer.CreateCell("0000002");
  std::unique_ptr<Cell::ICell> cell3 = hierarchicalGridIndexer.CreateCell("0000003");

  std::list<FaceCoordinate> centralVertices;
  hierarchicalGridIndexer.GetCellVertices(*centralCell, centralVertices);

  std::list<FaceCoordinate> cell1Vertices;
  hierarchicalGridIndexer.GetCellVertices(*cell1, cell1Vertices);
  std::list<FaceCoordinate> cell2Vertices;
  hierarchicalGridIndexer.GetCellVertices(*cell2, cell2Vertices);
  std::list<FaceCoordinate> cell3Vertices;
  hierarchicalGridIndexer.GetCellVertices(*cell3, cell3Vertices);

  // Check cell1
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell1Iter = cell1Vertices.begin();

    ++centralIter;
    FaceCoordinate centralVertex2 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex3 = *centralIter;

    // Central vertices 2 and 3 should match cell1 vertices 2 and 3 respectively
    ++cell1Iter;
    EXPECT_NEAR(centralVertex2.GetXOffset(), (*cell1Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex2.GetYOffset(), (*cell1Iter).GetYOffset(), TOLERANCE);
    ++cell1Iter;
    EXPECT_NEAR(centralVertex3.GetXOffset(), (*cell1Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex3.GetYOffset(), (*cell1Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell2
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell2Iter = cell2Vertices.begin();

    FaceCoordinate centralVertex1 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex2 = *centralIter;

    // Central vertices 1 and 2 should match cell2 vertices 3 and 1 respectively
    EXPECT_NEAR(centralVertex2.GetXOffset(), (*cell2Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex2.GetYOffset(), (*cell2Iter).GetYOffset(), TOLERANCE);
    ++cell2Iter;
    ++cell2Iter;
    EXPECT_NEAR(centralVertex1.GetXOffset(), (*cell2Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex1.GetYOffset(), (*cell2Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell3
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell3Iter = cell3Vertices.begin();

    FaceCoordinate centralVertex1 = *centralIter;
    ++centralIter;
    ++centralIter;
    FaceCoordinate centralVertex3 = *centralIter;

    // Central vertices 1 and 3 should match cell3 vertices 2 and 1 respectively
    EXPECT_NEAR(centralVertex3.GetXOffset(), (*cell3Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex3.GetYOffset(), (*cell3Iter).GetYOffset(), TOLERANCE);
    ++cell3Iter;
    EXPECT_NEAR(centralVertex1.GetXOffset(), (*cell3Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex1.GetYOffset(), (*cell3Iter).GetYOffset(), TOLERANCE);
  }
}

// This test proves requirement A.1.3
UNIT_TEST(OGC, CellBoundaryOverlapHexagon)
{
  // Find the vertex coordinates in lat/long of cells that touch a central cell
  // Each surrounding cell should have two coordinates in common with the central cell

  Grid::OffsetGrid::Aperture3HexagonGrid hexagonGrid;
  GridIndexer::OffsetGridIndexer offsetGridIndexer(&hexagonGrid, MAX_FACE_INDEX);

  // The central cell is in resolution 4 which has a vertically oriented hexagon
  // The cell is in an 'odd' row so is offset in x
  std::unique_ptr<Cell::ICell> centralCell = offsetGridIndexer.CreateCell("00041,1");
  std::unique_ptr<Cell::ICell> cell1 = offsetGridIndexer.CreateCell("00042,2");
  std::unique_ptr<Cell::ICell> cell2 = offsetGridIndexer.CreateCell("00041,2");
  std::unique_ptr<Cell::ICell> cell3 = offsetGridIndexer.CreateCell("00040,2");
  std::unique_ptr<Cell::ICell> cell4 = offsetGridIndexer.CreateCell("00040,1");
  std::unique_ptr<Cell::ICell> cell5 = offsetGridIndexer.CreateCell("00041,0");
  std::unique_ptr<Cell::ICell> cell6 = offsetGridIndexer.CreateCell("00042,1");

  std::list<FaceCoordinate> centralVertices;
  offsetGridIndexer.GetCellVertices(*centralCell, centralVertices);

  std::list<FaceCoordinate> cell1Vertices;
  offsetGridIndexer.GetCellVertices(*cell1, cell1Vertices);
  std::list<FaceCoordinate> cell2Vertices;
  offsetGridIndexer.GetCellVertices(*cell2, cell2Vertices);
  std::list<FaceCoordinate> cell3Vertices;
  offsetGridIndexer.GetCellVertices(*cell3, cell3Vertices);
  std::list<FaceCoordinate> cell4Vertices;
  offsetGridIndexer.GetCellVertices(*cell4, cell4Vertices);
  std::list<FaceCoordinate> cell5Vertices;
  offsetGridIndexer.GetCellVertices(*cell5, cell5Vertices);
  std::list<FaceCoordinate> cell6Vertices;
  offsetGridIndexer.GetCellVertices(*cell6, cell6Vertices);

  // Check cell1
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell1Iter = cell1Vertices.begin();

    FaceCoordinate centralVertex1 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex2 = *centralIter;

    // Central vertices 1 and 2 should match cell1 vertices 5 and 4 respectively
    ++cell1Iter;
    ++cell1Iter;
    ++cell1Iter;
    EXPECT_NEAR(centralVertex2.GetXOffset(), (*cell1Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex2.GetYOffset(), (*cell1Iter).GetYOffset(), TOLERANCE);
    ++cell1Iter;
    EXPECT_NEAR(centralVertex1.GetXOffset(), (*cell1Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex1.GetYOffset(), (*cell1Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell2
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell2Iter = cell2Vertices.begin();

    FaceCoordinate centralVertex1 = *centralIter;
    ++centralIter;
    ++centralIter;
    ++centralIter;
    ++centralIter;
    ++centralIter;
    FaceCoordinate centralVertex6 = *centralIter;

    // Central vertices 1 and 6 should match cell2 vertices 3 and 4 respectively
    ++cell2Iter;
    ++cell2Iter;
    EXPECT_NEAR(centralVertex1.GetXOffset(), (*cell2Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex1.GetYOffset(), (*cell2Iter).GetYOffset(), TOLERANCE);
    ++cell2Iter;
    EXPECT_NEAR(centralVertex6.GetXOffset(), (*cell2Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex6.GetYOffset(), (*cell2Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell3
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell3Iter = cell3Vertices.begin();

    ++centralIter;
    ++centralIter;
    ++centralIter;
    ++centralIter;
    FaceCoordinate centralVertex5 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex6 = *centralIter;

    // Central vertices 5 and 6 should match cell3 vertices 3 and 2 respectively
    ++cell3Iter;
    EXPECT_NEAR(centralVertex6.GetXOffset(), (*cell3Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex6.GetYOffset(), (*cell3Iter).GetYOffset(), TOLERANCE);
    ++cell3Iter;
    EXPECT_NEAR(centralVertex5.GetXOffset(), (*cell3Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex5.GetYOffset(), (*cell3Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell4
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell4Iter = cell4Vertices.begin();

    ++centralIter;
    ++centralIter;
    ++centralIter;
    FaceCoordinate centralVertex4 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex5 = *centralIter;

    // Central vertices 4 and 5 should match cell4 vertices 2 and 1 respectively
    EXPECT_NEAR(centralVertex5.GetXOffset(), (*cell4Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex5.GetYOffset(), (*cell4Iter).GetYOffset(), TOLERANCE);
    ++cell4Iter;
    EXPECT_NEAR(centralVertex4.GetXOffset(), (*cell4Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex4.GetYOffset(), (*cell4Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell5
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell5Iter = cell5Vertices.begin();

    ++centralIter;
    ++centralIter;
    FaceCoordinate centralVertex3 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex4 = *centralIter;

    // Central vertices 3 and 4 should match cell5 vertices 1 and 6 respectively
    EXPECT_NEAR(centralVertex3.GetXOffset(), (*cell5Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex3.GetYOffset(), (*cell5Iter).GetYOffset(), TOLERANCE);
    ++cell5Iter;
    ++cell5Iter;
    ++cell5Iter;
    ++cell5Iter;
    ++cell5Iter;
    EXPECT_NEAR(centralVertex4.GetXOffset(), (*cell5Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex4.GetYOffset(), (*cell5Iter).GetYOffset(), TOLERANCE);
  }

  // Check cell6
  {
    std::list<FaceCoordinate>::const_iterator centralIter = centralVertices.begin();
    std::list<FaceCoordinate>::const_iterator cell6Iter = cell6Vertices.begin();

    ++centralIter;
    FaceCoordinate centralVertex2 = *centralIter;
    ++centralIter;
    FaceCoordinate centralVertex3 = *centralIter;

    // Central vertices 2 and 3 should match cell6 vertices 6 and 5 respectively
    ++cell6Iter;
    ++cell6Iter;
    ++cell6Iter;
    ++cell6Iter;
    EXPECT_NEAR(centralVertex3.GetXOffset(), (*cell6Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex3.GetYOffset(), (*cell6Iter).GetYOffset(), TOLERANCE);
    ++cell6Iter;
    EXPECT_NEAR(centralVertex2.GetXOffset(), (*cell6Iter).GetXOffset(), TOLERANCE);
    EXPECT_NEAR(centralVertex2.GetYOffset(), (*cell6Iter).GetYOffset(), TOLERANCE);
  }
}

// This test proves requirement A.1.5
UNIT_TEST(OGC, AreaPreservationTriangle)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;
  GridIndexer::HierarchicalGridIndexer hierarchicalGridIndexer(&triangleGrid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> parentCell = hierarchicalGridIndexer.CreateCell("000000");
  std::unique_ptr<Cell::ICell> childCell1 = hierarchicalGridIndexer.CreateCell("0000000");
  std::unique_ptr<Cell::ICell> childCell2 = hierarchicalGridIndexer.CreateCell("0000001");
  std::unique_ptr<Cell::ICell> childCell3 = hierarchicalGridIndexer.CreateCell("0000002");
  std::unique_ptr<Cell::ICell> childCell4 = hierarchicalGridIndexer.CreateCell("0000003");

  std::list<FaceCoordinate> parentVertices;
  hierarchicalGridIndexer.GetCellVertices(*parentCell, parentVertices);

  std::list<FaceCoordinate> childCell1Vertices;
  hierarchicalGridIndexer.GetCellVertices(*childCell1, childCell1Vertices);
  std::list<FaceCoordinate> childCell2Vertices;
  hierarchicalGridIndexer.GetCellVertices(*childCell2, childCell2Vertices);
  std::list<FaceCoordinate> childCell3Vertices;
  hierarchicalGridIndexer.GetCellVertices(*childCell3, childCell3Vertices);
  std::list<FaceCoordinate> childCell4Vertices;
  hierarchicalGridIndexer.GetCellVertices(*childCell4, childCell4Vertices);

  double parentArea = GetPolygonArea(parentVertices);
  double childCell1Area = GetPolygonArea(childCell1Vertices);
  double childCell2Area = GetPolygonArea(childCell2Vertices);
  double childCell3Area = GetPolygonArea(childCell3Vertices);
  double childCell4Area = GetPolygonArea(childCell4Vertices);

  double totalChildArea = childCell1Area + childCell2Area + childCell3Area + childCell4Area;

  EXPECT_NEAR(totalChildArea, parentArea, TOLERANCE);
}

// This test proves requirement A.1.5
UNIT_TEST(OGC, AreaPreservationHexagon)
{
  Grid::OffsetGrid::Aperture3HexagonGrid hexagonGrid;
  GridIndexer::OffsetGridIndexer offsetGridIndexer(&hexagonGrid, MAX_FACE_INDEX);

  // Parent cell is in resolution 3 which has a horizontally oriented hexagon
  // The cell is in the 'odd' column so is offset in y
  std::unique_ptr<Cell::ICell> parentCell = offsetGridIndexer.CreateCell("00030,1");

  // Child cells are in resolution 4 which have vertically oriented hexagons
  std::unique_ptr<Cell::ICell> childCell1 = offsetGridIndexer.CreateCell("00042,2");
  std::unique_ptr<Cell::ICell> childCell2 = offsetGridIndexer.CreateCell("00041,2");
  std::unique_ptr<Cell::ICell> childCell3 = offsetGridIndexer.CreateCell("00040,2");
  std::unique_ptr<Cell::ICell> childCell4 = offsetGridIndexer.CreateCell("00040,1");
  std::unique_ptr<Cell::ICell> childCell5 = offsetGridIndexer.CreateCell("00041,0");
  std::unique_ptr<Cell::ICell> childCell6 = offsetGridIndexer.CreateCell("00042,1");
  std::unique_ptr<Cell::ICell> childCell7 = offsetGridIndexer.CreateCell("00042,1");

  std::list<FaceCoordinate> parentVertices;
  offsetGridIndexer.GetCellVertices(*parentCell, parentVertices);

  std::list<FaceCoordinate> childCell1Vertices;
  offsetGridIndexer.GetCellVertices(*childCell1, childCell1Vertices);
  std::list<FaceCoordinate> childCell2Vertices;
  offsetGridIndexer.GetCellVertices(*childCell2, childCell2Vertices);
  std::list<FaceCoordinate> childCell3Vertices;
  offsetGridIndexer.GetCellVertices(*childCell3, childCell3Vertices);
  std::list<FaceCoordinate> childCell4Vertices;
  offsetGridIndexer.GetCellVertices(*childCell4, childCell4Vertices);
  std::list<FaceCoordinate> childCell5Vertices;
  offsetGridIndexer.GetCellVertices(*childCell5, childCell5Vertices);
  std::list<FaceCoordinate> childCell6Vertices;
  offsetGridIndexer.GetCellVertices(*childCell6, childCell6Vertices);
  std::list<FaceCoordinate> childCell7Vertices;
  offsetGridIndexer.GetCellVertices(*childCell7, childCell7Vertices);

  double parentArea = GetPolygonArea(parentVertices);
  double childCell1Area = GetPolygonArea(childCell1Vertices);
  double childCell2Area = GetPolygonArea(childCell2Vertices);
  double childCell3Area = GetPolygonArea(childCell3Vertices);
  double childCell4Area = GetPolygonArea(childCell4Vertices);
  double childCell5Area = GetPolygonArea(childCell5Vertices);
  double childCell6Area = GetPolygonArea(childCell6Vertices);
  double childCell7Area = GetPolygonArea(childCell7Vertices);

  // Parent cell contains all of child 1 and 1/3 of all other cells
  double totalChildArea = childCell1Area + (childCell2Area + childCell3Area + childCell4Area + childCell5Area + childCell6Area + childCell7Area) / 3.0;

  EXPECT_NEAR(totalChildArea, parentArea, TOLERANCE);
}

// This test proves requirement A.1.9
UNIT_TEST(OGC, CellReferenceCentroidTriangle)
{
  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;
  GridIndexer::HierarchicalGridIndexer hierarchicalGridIndexer(&triangleGrid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = hierarchicalGridIndexer.CreateCell("0000000");

  std::list<FaceCoordinate> vertices;
  hierarchicalGridIndexer.GetCellVertices(*cell, vertices);

  // Calculate the centroid of the cell
  CartesianPoint centroid = GetPolygonCentroid(vertices);

  PolyhedralGlobe::Icosahedron icosahedron;
  Projection::Snyder snyder(&icosahedron);
  EAGGR::Model::DGGS dggs(&snyder, &hierarchicalGridIndexer);

  // Use the DGGS to convert the cell to a reference point in the lat/long domain
  EAGGR::LatLong::SphericalAccuracyPoint point = dggs.ConvertCellToLatLongPoint(*cell);

  // Use the projection to convert the cell reference point to a face coordinate
  FaceCoordinate referenceFaceCoordinate = snyder.GetFaceCoordinate(point);

  // The cell centroid and the reference face coordinate should match
  EXPECT_NEAR(centroid.GetX(), referenceFaceCoordinate.GetXOffset(), TOLERANCE);
  EXPECT_NEAR(centroid.GetY(), referenceFaceCoordinate.GetYOffset(), TOLERANCE);
}

// This test proves requirement A.1.9
UNIT_TEST(OGC, CellReferenceCentroidHexagon)
{
  Grid::OffsetGrid::Aperture3HexagonGrid hexagonGrid;
  GridIndexer::OffsetGridIndexer offsetGridIndexer(&hexagonGrid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = offsetGridIndexer.CreateCell("00041,1");

  std::list<FaceCoordinate> vertices;
  offsetGridIndexer.GetCellVertices(*cell, vertices);

  // Calculate the centroid of the cell
  CartesianPoint centroid = GetPolygonCentroid(vertices);

  PolyhedralGlobe::Icosahedron icosahedron;
  Projection::Snyder snyder(&icosahedron);
  EAGGR::Model::DGGS dggs(&snyder, &offsetGridIndexer);

  // Use the DGGS to convert the cell to a reference point in the lat/long domain
  EAGGR::LatLong::SphericalAccuracyPoint point = dggs.ConvertCellToLatLongPoint(*cell);

  // Use the projection to convert the cell reference point to a face coordinate
  FaceCoordinate referenceFaceCoordinate = snyder.GetFaceCoordinate(point);

  // The cell centroid and the reference face coordinate should match
  EXPECT_NEAR(centroid.GetX(), referenceFaceCoordinate.GetXOffset(), TOLERANCE);
  EXPECT_NEAR(centroid.GetY(), referenceFaceCoordinate.GetYOffset(), TOLERANCE);
}

// This test proves requirement A.1.2
UNIT_TEST(OGC, SurfaceAreaEquivalence)
{
  PolyhedralGlobe::Icosahedron icosahedron;
  Grid::HierarchicalGrid::Aperture4TriangleGrid triangleGrid;
  GridIndexer::HierarchicalGridIndexer hierarchicalGridIndexer(&triangleGrid, MAX_FACE_INDEX);

  std::unique_ptr<Cell::ICell> cell = hierarchicalGridIndexer.CreateCell("01");
  std::list<FaceCoordinate> vertices;
  hierarchicalGridIndexer.GetCellVertices(*cell, vertices);

  // Need to scale the side length of the icosahedron face to be tangential to the earth
  // By default icosahedron face has side length of 1. Scale this by the Earth Radius and
  // the icosahedron scaling factors to get the side length.
  double lengthScalingFactor = RADIUS_OF_EARTH * icosahedron.GetRPrimeRelativeToR()
  * icosahedron.GetEdgeLengthRelativeToRPrime();

  double area = GetPolygonArea(vertices) * lengthScalingFactor * lengthScalingFactor;

  // There are 20 faces of the icosahedron
  double totalArea = 20.0 * area;

  EXPECT_NEAR(SURFACE_AREA_OF_EARTH, totalArea, 1E10);
}
