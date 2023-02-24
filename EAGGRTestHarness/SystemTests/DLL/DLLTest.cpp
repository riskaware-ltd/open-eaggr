//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: System Tests
//
//------------------------------------------------------
/// @file DLLTest.cpp
/// 
/// System tests for the DGGS DLL.
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

#include <cstdio>
#include <fstream>
#include <cstring>

#include "TestMacros.hpp"

#include "../EAGGR/API/eaggr_api.h"

#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/LatLong/IShape.hpp"
#include "Src/LatLong/LatLongShape.hpp"
#include "Src/ImportExport/AbstractShapeImporter.hpp"
#include "Src/ImportExport/GeoJsonImporter.hpp"
#include "Src/ImportExport/WktImporter.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/EAGGRException.hpp"

#include "TestUtilities/KmlFileMatcher.hpp"

static const double LAT_LONG_TOLERANCE = 1e-3;
static const double ACCURACY_TOLERANCE = 1e-8;

using namespace EAGGR;

void CheckShapeString(ImportExport::AbstractShapeImporter * a_importer);

void AssignCell(DGGS_Cell destination, const DGGS_Cell source)
{
  strncpy(destination, source, EAGGR_MAX_CELL_STRING_LENGTH);
}

class TestData
{
  public:
    TestData()
    {
      // Set up base shapes
      m_baseCell.m_type = DGGS_CELL;
      m_baseCell.m_location = DGGS_ONE_FACE;
      AssignCell(m_baseCell.m_data.m_cell, "0700");

      m_baseLinestring.m_type = DGGS_LINESTRING;
      m_baseLinestring.m_location = DGGS_ONE_FACE;
      m_baseLinestring.m_data.m_linestring.m_noOfCells = 3U;
      AssignCell(baseLinestringCellIds[0], "0701");
      AssignCell(baseLinestringCellIds[1], "0702");
      AssignCell(baseLinestringCellIds[2], "0703");
      m_baseLinestring.m_data.m_linestring.m_cells = baseLinestringCellIds;

      m_basePolygon.m_type = DGGS_POLYGON;
      m_basePolygon.m_location = DGGS_ONE_FACE;
      m_basePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 4U;
      AssignCell(basePolygonOuterLinestringCellIds[0], "07111");
      AssignCell(basePolygonOuterLinestringCellIds[1], "0702");
      AssignCell(basePolygonOuterLinestringCellIds[2], "0701");
      AssignCell(basePolygonOuterLinestringCellIds[3], "07333");
      m_basePolygon.m_data.m_polygon.m_outerRing.m_cells = basePolygonOuterLinestringCellIds;
      m_basePolygon.m_data.m_polygon.m_noOfInnerRings = 1U;
      AssignCell(basePolygonInnerRingLinestringCellIds[0], "07312");
      AssignCell(basePolygonInnerRingLinestringCellIds[1], "07302");
      AssignCell(basePolygonInnerRingLinestringCellIds[2], "07321");
      basePolygonInnerRingLinestring[0].m_noOfCells = 3U;
      basePolygonInnerRingLinestring[0].m_cells = basePolygonInnerRingLinestringCellIds;
      m_basePolygon.m_data.m_polygon.m_innerRings = basePolygonInnerRingLinestring;

      // Set up disjoint shapes
      m_disjointCell.m_type = DGGS_CELL;
      m_disjointCell.m_location = DGGS_ONE_FACE;
      AssignCell(m_disjointCell.m_data.m_cell, "0711");

      m_disjointLinestring.m_type = DGGS_LINESTRING;
      m_disjointLinestring.m_location = DGGS_ONE_FACE;
      m_disjointLinestring.m_data.m_linestring.m_noOfCells = 3U;
      AssignCell(disjointLinestringCellIds[0], "0721");
      AssignCell(disjointLinestringCellIds[1], "0722");
      AssignCell(disjointLinestringCellIds[2], "0723");
      m_disjointLinestring.m_data.m_linestring.m_cells = disjointLinestringCellIds;

      m_disjointPolygon.m_type = DGGS_POLYGON;
      m_disjointPolygon.m_location = DGGS_ONE_FACE;
      m_disjointPolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
      m_disjointPolygon.m_data.m_polygon.m_outerRing.m_cells = disjointLinestringCellIds;
      m_disjointPolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

      // Other shapes
      m_otherCell.m_type = DGGS_CELL;
      m_otherCell.m_location = DGGS_ONE_FACE;
      AssignCell(m_otherCell.m_data.m_cell, "0701");

      m_childCell.m_type = DGGS_CELL;
      m_childCell.m_location = DGGS_ONE_FACE;
      AssignCell(m_childCell.m_data.m_cell, "07000");

      m_parentCell.m_type = DGGS_CELL;
      m_parentCell.m_location = DGGS_ONE_FACE;
      AssignCell(m_parentCell.m_data.m_cell, "070");

      m_substringLinestring.m_type = DGGS_LINESTRING;
      m_substringLinestring.m_location = DGGS_ONE_FACE;
      m_substringLinestring.m_data.m_linestring.m_noOfCells = 2U;
      AssignCell(substringLinestringCellIds[0], "0701");
      AssignCell(substringLinestringCellIds[1], "0702");
      m_substringLinestring.m_data.m_linestring.m_cells = substringLinestringCellIds;

      m_noInnerRingsPolygon.m_type = DGGS_POLYGON;
      m_noInnerRingsPolygon.m_location = DGGS_ONE_FACE;
      m_noInnerRingsPolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
      m_noInnerRingsPolygon.m_data.m_polygon.m_outerRing.m_cells =
          basePolygonOuterLinestringCellIds;
      m_noInnerRingsPolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

      m_containsPolygon.m_type = DGGS_POLYGON;
      m_containsPolygon.m_location = DGGS_ONE_FACE;
      m_containsPolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
      AssignCell(containsOuterLinestringCellIds[0], "07111");
      AssignCell(containsOuterLinestringCellIds[1], "07222");
      AssignCell(containsOuterLinestringCellIds[2], "07333");
      m_containsPolygon.m_data.m_polygon.m_outerRing.m_cells = containsOuterLinestringCellIds;
      m_containsPolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

      m_interiorPolygon.m_type = DGGS_POLYGON;
      m_interiorPolygon.m_location = DGGS_ONE_FACE;
      m_interiorPolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
      AssignCell(interiorOuterLinestringCellIds[0], "07101");
      AssignCell(interiorOuterLinestringCellIds[1], "07132");
      AssignCell(interiorOuterLinestringCellIds[2], "07001");
      m_interiorPolygon.m_data.m_polygon.m_outerRing.m_cells = interiorOuterLinestringCellIds;
      m_interiorPolygon.m_data.m_polygon.m_noOfInnerRings = 0U;
    }

    DGGS_Shape m_baseCell;
    DGGS_Shape m_baseLinestring;
    DGGS_Shape m_basePolygon;

    DGGS_Shape m_disjointCell;
    DGGS_Shape m_disjointLinestring;
    DGGS_Shape m_disjointPolygon;

    DGGS_Shape m_otherCell;
    DGGS_Shape m_childCell;
    DGGS_Shape m_parentCell;
    DGGS_Shape m_substringLinestring;
    DGGS_Shape m_noInnerRingsPolygon;
    DGGS_Shape m_containsPolygon;
    DGGS_Shape m_interiorPolygon;

  private:
    DGGS_Cell baseLinestringCellIds[3];
    DGGS_Cell basePolygonOuterLinestringCellIds[4];
    DGGS_Cell basePolygonInnerRingLinestringCellIds[3];
    DGGS_Linestring basePolygonInnerRingLinestring[1];

    DGGS_Cell disjointLinestringCellIds[3];
    DGGS_Cell substringLinestringCellIds[2];

    DGGS_Cell containsOuterLinestringCellIds[3];
    DGGS_Cell interiorOuterLinestringCellIds[3];
};

SYSTEM_TEST(DLL, EAGGR_GetVersion)
{
  DGGS_ReturnCode returnCode;

  char versionString[EAGGR_VERSION_STRING_LENGTH] = "";
  returnCode = EAGGR_GetVersion(versionString);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("v2.0", versionString);

  // Test error cases
  returnCode = EAGGR_GetVersion(NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_GetLastErrorMessage)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  char * errorMessage = NULL;
  unsigned short messageLength = 0U;

  // Test error cases
  returnCode = EAGGR_GetLastErrorMessage(NULL, &errorMessage, &messageLength);
  EAGGR_DeallocateString(handle, &errorMessage);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_GetLastErrorMessage(handle, NULL, &messageLength);
  EAGGR_DeallocateString(handle, &errorMessage);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, NULL);
  EAGGR_DeallocateString(handle, &errorMessage);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Get the message for the last error case
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  static const char EXPECTED_ERROR_MSG[] = "Pointer argument 'a_pMessageLength' is null";
  EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);
}

SYSTEM_TEST(DLL, EAGGR_DeallocateString)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  char * errorMessage = NULL;
  unsigned short messageLength = 0U;

  // Test error cases
  returnCode = EAGGR_DeallocateString(NULL, &errorMessage);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_DeallocateString(handle, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Generate an error message by calling a function with a null pointer
  returnCode = EAGGR_ConvertPointsToDggsCells(handle, NULL, 1, NULL);
  EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  EXPECT_GT(messageLength, 0U);

  // Deallocate the error message and check that the pointer is set to zero
  returnCode = EAGGR_DeallocateString(handle, &errorMessage);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_EQ(NULL, errorMessage);

  // Check that it doesn't try to free a null pointer
  returnCode = EAGGR_DeallocateString(handle, &errorMessage);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_EQ(NULL, errorMessage);
}

SYSTEM_TEST(DLL, EAGGR_OpenDggsHandle)
{
  DGGS_ReturnCode returnCode;

  // Test error cases
  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_CloseDggsHandle)
{
  DGGS_ReturnCode returnCode;

  DGGS_Handle nullHandle = NULL;

  // Test error cases
  returnCode = EAGGR_CloseDggsHandle(NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_CloseDggsHandle(&nullHandle);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertPointsToDggsCells)
{
  static const unsigned short NO_OF_POINTS = 2U;
  DGGS_LatLongPoint latLongPoints[NO_OF_POINTS] =
  {
    { 1.234, 2.345, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)},
    { 3.456, 4.567, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)}
  };

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Cell cells[NO_OF_POINTS];
  returnCode = EAGGR_ConvertPointsToDggsCells(handle, latLongPoints, NO_OF_POINTS, cells);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("07231131111113100331001", cells[0]);
  EXPECT_STREQ("07012000001303022011321", cells[1]);

  // Test null pointer error cases7
  returnCode = EAGGR_ConvertPointsToDggsCells(NULL, latLongPoints, NO_OF_POINTS, cells);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertPointsToDggsCells(handle, NULL, NO_OF_POINTS, cells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertPointsToDggsCells(handle, latLongPoints, NO_OF_POINTS, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Pass in an invalid lat / long point
  DGGS_LatLongPoint invalidPoint =
  { 360.0, 360.0, 0.0};

  // Test the invalid point is handled correctly
  returnCode = EAGGR_ConvertPointsToDggsCells(handle, &invalidPoint, 1U, cells);
  ASSERT_EQ(DGGS_MODEL_ERROR, returnCode);

  // Get the error message
  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Check the error message
  static const char EXPECTED_ERROR_MSG[]
  = "EAGGR Exception: Latitude is greater than maximum allowed for a lat/long point.";
  EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertShapesToDggsShapes)
{
  DGGS_LatLongPoint point1 =
  { 1.234, 2.345, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};
  DGGS_LatLongPoint point2 =
  { 3.456, 4.567, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};

  static const unsigned short NO_OF_SHAPES = 4U;
  DGGS_LatLongShape latLongShapes[NO_OF_SHAPES];

  // Add two points to the shapes
  latLongShapes[0].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[0].m_data.m_point = point1;
  latLongShapes[1].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[1].m_data.m_point = point2;

  // Add one linestring to the shapes
  latLongShapes[2].m_type = DGGS_LAT_LONG_LINESTRING;

  static const unsigned short NO_OF_LINESTRING_POINTS = 2U;
  latLongShapes[2].m_data.m_linestring.m_noOfPoints = NO_OF_LINESTRING_POINTS;
  latLongShapes[2].m_data.m_linestring.m_points
  = static_cast<DGGS_LatLongPoint *>(malloc(NO_OF_LINESTRING_POINTS * sizeof(DGGS_LatLongPoint)));
  latLongShapes[2].m_data.m_linestring.m_points[0] = point1;
  latLongShapes[2].m_data.m_linestring.m_points[1] = point2;

  // Add one polygon to the shapes
  // Note: Uses the same points for both the outer and inner rings so it is not
  // technically a valid polygon, but fine for testing the conversion.
  latLongShapes[3].m_type = DGGS_LAT_LONG_POLYGON;

  static const unsigned short NO_OF_OUTER_RING_POINTS = 2U;
  latLongShapes[3].m_data.m_polygon.m_outerRing.m_noOfPoints = NO_OF_OUTER_RING_POINTS;
  latLongShapes[3].m_data.m_polygon.m_outerRing.m_points
  = static_cast<DGGS_LatLongPoint *>(malloc(NO_OF_OUTER_RING_POINTS * sizeof(DGGS_LatLongPoint)));
  latLongShapes[3].m_data.m_polygon.m_outerRing.m_points[0] = point1;
  latLongShapes[3].m_data.m_polygon.m_outerRing.m_points[1] = point2;

  static const unsigned short NO_OF_POLYGON_INNER_RINGS = 2U;
  latLongShapes[3].m_data.m_polygon.m_noOfInnerRings = NO_OF_POLYGON_INNER_RINGS;
  latLongShapes[3].m_data.m_polygon.m_innerRings
  = static_cast<DGGS_LatLongLinestring *>(malloc(NO_OF_POLYGON_INNER_RINGS * sizeof(DGGS_LatLongLinestring)));

  static const unsigned short NO_OF_1ST_INNER_RING_POINTS = 2U;
  latLongShapes[3].m_data.m_polygon.m_innerRings[0].m_noOfPoints = NO_OF_1ST_INNER_RING_POINTS;
  latLongShapes[3].m_data.m_polygon.m_innerRings[0].m_points
  = static_cast<DGGS_LatLongPoint *>(malloc(NO_OF_1ST_INNER_RING_POINTS * sizeof(DGGS_LatLongPoint)));
  latLongShapes[3].m_data.m_polygon.m_innerRings[0].m_points[0] = point1;
  latLongShapes[3].m_data.m_polygon.m_innerRings[0].m_points[1] = point2;

  static const unsigned short NO_OF_2ND_INNER_RING_POINTS = 2U;
  latLongShapes[3].m_data.m_polygon.m_innerRings[1].m_noOfPoints = NO_OF_2ND_INNER_RING_POINTS;
  latLongShapes[3].m_data.m_polygon.m_innerRings[1].m_points
  = static_cast<DGGS_LatLongPoint *>(malloc(NO_OF_2ND_INNER_RING_POINTS * sizeof(DGGS_LatLongPoint)));
  latLongShapes[3].m_data.m_polygon.m_innerRings[1].m_points[0] = point1;
  latLongShapes[3].m_data.m_polygon.m_innerRings[1].m_points[1] = point2;

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Shape * shapes;
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(DGGS_CELL, shapes[0].m_type);
  EXPECT_STREQ("07231131111113100331001", shapes[0].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[0].m_location);

  EXPECT_EQ(DGGS_CELL, shapes[1].m_type);
  EXPECT_STREQ("07012000001303022011321", shapes[1].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[1].m_location);

  EXPECT_EQ(DGGS_LINESTRING, shapes[2].m_type);
  EXPECT_EQ(DGGS_NO_LOCATION, shapes[2].m_location);
  EXPECT_EQ(NO_OF_LINESTRING_POINTS, shapes[2].m_data.m_linestring.m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[2].m_data.m_linestring.m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[2].m_data.m_linestring.m_cells[1]);

  EXPECT_EQ(DGGS_POLYGON, shapes[3].m_type);
  EXPECT_EQ(DGGS_NO_LOCATION, shapes[3].m_location);
  EXPECT_EQ(NO_OF_OUTER_RING_POINTS, shapes[3].m_data.m_polygon.m_outerRing.m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_outerRing.m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_outerRing.m_cells[1]);
  EXPECT_EQ(NO_OF_POLYGON_INNER_RINGS, shapes[3].m_data.m_polygon.m_noOfInnerRings);
  EXPECT_EQ(NO_OF_1ST_INNER_RING_POINTS, shapes[3].m_data.m_polygon.m_innerRings[0].m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_innerRings[0].m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_innerRings[0].m_cells[1]);
  EXPECT_EQ(NO_OF_2ND_INNER_RING_POINTS, shapes[3].m_data.m_polygon.m_innerRings[1].m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_innerRings[1].m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_innerRings[1].m_cells[1]);

  returnCode = EAGGR_DeallocateDggsShapes(handle, &shapes, NO_OF_SHAPES);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Test null pointer error cases
  returnCode = EAGGR_ConvertShapesToDggsShapes(NULL, latLongShapes, NO_OF_SHAPES, &shapes);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, NULL, NO_OF_SHAPES, &shapes);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Make the first lat / long point invalid
  DGGS_LatLongPoint invalidPoint =
  { 360.0, 360.0, 0.0};
  latLongShapes[0].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[0].m_data.m_point = invalidPoint;

  // Test the invalid point is handled correctly
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
  ASSERT_EQ(DGGS_MODEL_ERROR, returnCode);

  // Get the error message
  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Check the error message
  static const char EXPECTED_ERROR_MSG[]
  = "EAGGR Exception: Latitude is greater than maximum allowed for a lat/long point.";
  EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertShapeStringToDggsShapes)
{
  char wktString[] =
  "GEOMETRYCOLLECTION("
  "MULTIPOINT(2.345 1.234, 4.567 3.456), "
  "LINESTRING(2.345 1.234, 4.567 3.456),"
  "POLYGON((2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456)))";

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Shape * shapes;
  unsigned short noOfShapes = 0U;
  returnCode = EAGGR_ConvertShapeStringToDggsShapes(handle, wktString, DGGS_WKT_FORMAT,
      LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5), &shapes, &noOfShapes);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  ASSERT_EQ(4U, noOfShapes);

  ASSERT_EQ(DGGS_CELL, shapes[0].m_type);
  EXPECT_STREQ("07231131111113100331001", shapes[0].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[0].m_location);

  ASSERT_EQ(DGGS_CELL, shapes[1].m_type);
  EXPECT_STREQ("07012000001303022011321", shapes[1].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[1].m_location);

  ASSERT_EQ(DGGS_LINESTRING, shapes[2].m_type);
  EXPECT_EQ(DGGS_NO_LOCATION, shapes[2].m_location);
  ASSERT_EQ(2U, shapes[2].m_data.m_linestring.m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[2].m_data.m_linestring.m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[2].m_data.m_linestring.m_cells[1]);

  ASSERT_EQ(DGGS_POLYGON, shapes[3].m_type);
  EXPECT_EQ(DGGS_NO_LOCATION, shapes[3].m_location);
  ASSERT_EQ(2U, shapes[3].m_data.m_polygon.m_outerRing.m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_outerRing.m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_outerRing.m_cells[1]);
  ASSERT_EQ(2U, shapes[3].m_data.m_polygon.m_noOfInnerRings);
  ASSERT_EQ(2U, shapes[3].m_data.m_polygon.m_innerRings[0].m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_innerRings[0].m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_innerRings[0].m_cells[1]);
  ASSERT_EQ(2U, shapes[3].m_data.m_polygon.m_innerRings[1].m_noOfCells);
  EXPECT_STREQ("07231131111113100331001", shapes[3].m_data.m_polygon.m_innerRings[1].m_cells[0]);
  EXPECT_STREQ("07012000001303022011321", shapes[3].m_data.m_polygon.m_innerRings[1].m_cells[1]);

  returnCode = EAGGR_DeallocateDggsShapes(handle, &shapes, noOfShapes);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Test error cases for EAGGR_ConvertShapeStringToDggsShapes()
  returnCode = EAGGR_ConvertShapeStringToDggsShapes(NULL, wktString, DGGS_WKT_FORMAT, 1.0e-5, &shapes, &noOfShapes);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertShapeStringToDggsShapes(handle, NULL, DGGS_WKT_FORMAT, 1.0e-5, &shapes, &noOfShapes);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertShapeStringToDggsShapes(handle, wktString, DGGS_WKT_FORMAT, 1.0e-5, NULL, &noOfShapes);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertShapeStringToDggsShapes(handle, wktString, DGGS_WKT_FORMAT, 1.0e-5, &shapes, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Test error cases for EAGGR_DeallocateDggsShapes()
  returnCode = EAGGR_DeallocateDggsShapes(NULL, &shapes, noOfShapes);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_DeallocateDggsShapes(handle, NULL, noOfShapes);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertDggsCellsToPoints)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cells[] =
  {
    "07231131111113100331032", "07012000001303022011321"
  };
  const unsigned short noOfCells = sizeof(cells) / sizeof(cells[0]);

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_LatLongPoint points[noOfCells];
  returnCode = EAGGR_ConvertDggsCellsToPoints(handle, cells, noOfCells, points);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  unsigned short pointIndex = 0U;
  EXPECT_NEAR(1.234, points[pointIndex].m_latitude, LAT_LONG_TOLERANCE);
  EXPECT_NEAR(2.345, points[pointIndex].m_longitude, LAT_LONG_TOLERANCE);
  EXPECT_NEAR(1.222e-05,
      LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(points[pointIndex].m_accuracy), ACCURACY_TOLERANCE);

  pointIndex++;
  EXPECT_NEAR(3.456, points[pointIndex].m_latitude, LAT_LONG_TOLERANCE);
  EXPECT_NEAR(4.567, points[pointIndex].m_longitude, LAT_LONG_TOLERANCE);
  EXPECT_NEAR(1.222e-05,
      LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(points[pointIndex].m_accuracy), ACCURACY_TOLERANCE);

  // Test null pointer error cases
  returnCode = EAGGR_ConvertDggsCellsToPoints(NULL, cells, noOfCells, points);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertDggsCellsToPoints(handle, NULL, noOfCells, points);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertDggsCellsToPoints(handle, cells, noOfCells, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Make the first DGGS cell invalid
  cells[0][0] = 'X';

  // Test the invalid cell is handled correctly
  returnCode = EAGGR_ConvertDggsCellsToPoints(handle, cells, noOfCells, points);
  ASSERT_EQ(DGGS_MODEL_ERROR, returnCode);

  // Get the error message
  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Check the error message
  static const char EXPECTED_ERROR_MSG[]
  = "EAGGR Exception: Invalid face index, 'X7', must be positive integer";
  EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertDggsCellsToShapeString)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cells[] =
  {
    "07231131111113100331032", "07012000001303022011321"
  };
  const unsigned short noOfCells = sizeof(cells) / sizeof(cells[0]);

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  char * pString;

  returnCode = EAGGR_ConvertDggsCellsToShapeString(handle, cells, noOfCells, DGGS_WKT_FORMAT, &pString);
  EXPECT_EQ(DGGS_SUCCESS, returnCode);

  // Import the string and test the values numerically because values in the string can vary slightly
  ImportExport::WktImporter wktImporter(pString, 1.0e-5);
  CheckShapeString(&wktImporter);
  EAGGR_DeallocateString(handle, &pString);

  returnCode = EAGGR_ConvertDggsCellsToShapeString(handle, cells, noOfCells, DGGS_GEO_JSON_FORMAT, &pString);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Import the string and test the values numerically because values in the string can vary slightly
  ImportExport::GeoJsonImporter geoJsonImporter(pString, 1.0e-5);
  CheckShapeString(&geoJsonImporter);
  EAGGR_DeallocateString(handle, &pString);

  // Test error cases
  returnCode = EAGGR_ConvertDggsCellsToShapeString(NULL, cells, noOfCells, DGGS_WKT_FORMAT, &pString);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertDggsCellsToShapeString(handle, NULL, noOfCells, DGGS_WKT_FORMAT, &pString);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertDggsCellsToShapeString(handle, cells, noOfCells, DGGS_WKT_FORMAT, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_GetDggsCellParents)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cell = "07012212222221011101013";

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Cell parentCells[EAGGR_MAX_PARENT_CELLS] =
  {};
  unsigned short noOfParentCells = 0U;

  returnCode = EAGGR_GetDggsCellParents(handle, cell, parentCells, &noOfParentCells);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(1U, noOfParentCells);
  EXPECT_STREQ("0701221222222101110101", parentCells[0]);

  // Test error cases
  returnCode = EAGGR_GetDggsCellParents(NULL, cell, parentCells, &noOfParentCells);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_GetDggsCellParents(handle, cell, NULL, &noOfParentCells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_GetDggsCellParents(handle, cell, parentCells, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_GetDggsCellChildren)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cell = "07012212222221011101013";

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Cell childCells[EAGGR_MAX_CHILD_CELLS] =
  {};
  unsigned short noOfChildCells = 0U;

  returnCode = EAGGR_GetDggsCellChildren(handle, cell, childCells, &noOfChildCells);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(4U, noOfChildCells);
  EXPECT_STREQ("070122122222210111010130", childCells[0]);
  EXPECT_STREQ("070122122222210111010131", childCells[1]);
  EXPECT_STREQ("070122122222210111010132", childCells[2]);
  EXPECT_STREQ("070122122222210111010133", childCells[3]);

  // Test error cases
  returnCode = EAGGR_GetDggsCellChildren(NULL, cell, childCells, &noOfChildCells);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_GetDggsCellChildren(handle, cell, NULL, &noOfChildCells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_GetDggsCellChildren(handle, cell, childCells, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Create a valid parent cell that produces child IDs that are too long
  // Note this is a cell at resolution 40 due to the restriction on cell resolution
  DGGS_Cell parentCell = "012301230123012301230123012301230123012301";

  char * errorMessage;
  unsigned short messageLength = 0U;
  DGGS_Cell cellArray;
  unsigned short noOfcellsInArray = 0U;

  returnCode = EAGGR_GetDggsCellChildren(handle, parentCell, &cellArray, &noOfcellsInArray);
  ASSERT_EQ(DGGS_MODEL_ERROR, returnCode);

  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  static const char EXPECTED_CHILD_ERROR_MSG[] =
  "EAGGR Exception: Unable to create cell at resolution 41 as it is greater than the upper limit (40).";
  EXPECT_EQ(sizeof(EXPECTED_CHILD_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_CHILD_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_GetDggsCellSiblings)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cell = "070122122222210111010130";

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Cell siblingCells[EAGGR_MAX_SIBLING_CELLS] =
  {};
  unsigned short noOfSiblingCells = 0U;

  returnCode = EAGGR_GetDggsCellSiblings(handle, cell, siblingCells, &noOfSiblingCells);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(3U, noOfSiblingCells);
  EXPECT_STREQ("070122122222210111010131", siblingCells[0]);
  EXPECT_STREQ("070122122222210111010132", siblingCells[1]);
  EXPECT_STREQ("070122122222210111010133", siblingCells[2]);

  // Test error cases
  returnCode = EAGGR_GetDggsCellSiblings(NULL, cell, siblingCells, &noOfSiblingCells);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_GetDggsCellSiblings(handle, cell, NULL, &noOfSiblingCells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_GetDggsCellSiblings(handle, cell, siblingCells, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertShapesToDggsShapesISEA3H)
{
  static const unsigned short NO_OF_SHAPES = 4U;

  DGGS_LatLongPoint point1 =
  { 1.234, 2.345, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};
  DGGS_LatLongPoint point2 =
  { 3.456, 4.567, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};
  DGGS_LatLongPoint point3 =
  { 80.0, 180.0, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};
  DGGS_LatLongPoint point4 =
  { 90.0, 0.0, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.0e-5)};

  DGGS_LatLongShape latLongShapes[NO_OF_SHAPES];
  latLongShapes[0].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[0].m_data.m_point = point1;
  latLongShapes[1].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[1].m_data.m_point = point2;
  latLongShapes[2].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[2].m_data.m_point = point3;
  latLongShapes[3].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[3].m_data.m_point = point4;

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA3H, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Shape * shapes;
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(DGGS_CELL, shapes[0].m_type);
  EXPECT_STREQ("0728-549628,-522499", shapes[0].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[0].m_location);

  EXPECT_EQ(DGGS_CELL, shapes[1].m_type);
  EXPECT_STREQ("0728-592991,-296234", shapes[1].m_data.m_cell);
  EXPECT_EQ(DGGS_ONE_FACE, shapes[1].m_location);

  EXPECT_EQ(DGGS_CELL, shapes[2].m_type);
  EXPECT_STREQ("00282407786,-390430", shapes[2].m_data.m_cell);
  EXPECT_EQ(DGGS_TWO_FACES, shapes[2].m_location);

  EXPECT_EQ(DGGS_CELL, shapes[3].m_type);
  EXPECT_STREQ("00283188646,0", shapes[3].m_data.m_cell);
  EXPECT_EQ(DGGS_MANY_FACES, shapes[3].m_location);

  returnCode = EAGGR_DeallocateDggsShapes(handle, &shapes, NO_OF_SHAPES);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Test null pointer error cases
  returnCode = EAGGR_ConvertShapesToDggsShapes(NULL, latLongShapes, NO_OF_SHAPES, &shapes);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, NULL, NO_OF_SHAPES, &shapes);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Make the first lat / long point invalid
  DGGS_LatLongPoint invalidPoint =
  { 360.0, 360.0, 0.0};
  latLongShapes[0].m_type = DGGS_LAT_LONG_POINT;
  latLongShapes[0].m_data.m_point = invalidPoint;

  // Test the invalid point is handled correctly
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
  ASSERT_EQ(DGGS_MODEL_ERROR, returnCode);

  // Get the error message
  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Check the error message
  static const char EXPECTED_ERROR_MSG[]
  = "EAGGR Exception: Latitude is greater than maximum allowed for a lat/long point.";
  EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
  EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_CreateDggsKmlFile)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  const char filename[] = "OpenEAGGR/EAGGRTestHarness/TestData/ActualKmlFile.kml";
  DGGS_Cell cells[] =
  {
    "07231131111113100331001", "07012000001303022011321"
  };
  const unsigned short noOfCells = sizeof(cells) / sizeof(cells[0]);

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  returnCode = EAGGR_CreateDggsKmlFile(handle, filename, cells, noOfCells);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Read expected output file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/DLL_CreateDggsKmlFile.kml");
  if (!expectedFile.is_open())
  {
    // Unable to open file containing expected KML
    FAIL();
  }

  // Read actual output file
  std::ifstream actualFile;
  actualFile.open(filename);

  // Compare the contents of the two files
  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualFile);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }

  // Close the files
  expectedFile.close();
  actualFile.close();

  // Delete the output KML file
  std::remove(filename);

  // Test error cases
  returnCode = EAGGR_CreateDggsKmlFile(NULL, filename, cells, noOfCells);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_CreateDggsKmlFile(handle, NULL, cells, noOfCells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);
  returnCode = EAGGR_CreateDggsKmlFile(handle, filename, NULL, noOfCells);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  // Close the DGGS handle
  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, EAGGR_ConvertDggsCellOutlineToShapeString)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_Cell cell = "07231131111113100331001";

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  char * pString;

  returnCode = EAGGR_ConvertDggsCellOutlineToShapeString(handle, cell, DGGS_WKT_FORMAT, &pString);
  EXPECT_EQ(DGGS_SUCCESS, returnCode);

  // Import the string and test the values numerically because values in the string can vary slightly
  ImportExport::WktImporter wktImporter(pString, 1.0e-5);
  LatLong::LatLongShape shape = wktImporter.GetNextShape();
  ASSERT_EQ(LatLong::WGS84_POLYGON, shape.GetShapeType());

  const LatLong::Wgs84Polygon * pPoint =
  static_cast<const LatLong::Wgs84Polygon *>(shape.GetShapeData());
  ASSERT_EQ(3, pPoint->GetOuterRing()->GetNumberOfPoints());
  EXPECT_NEAR(1.2340080, pPoint->GetOuterRing()->GetAccuracyPoint(0)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3449866, pPoint->GetOuterRing()->GetAccuracyPoint(0)->GetLongitude(), 1E-7);
  EXPECT_NEAR(1.2339790, pPoint->GetOuterRing()->GetAccuracyPoint(1)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3450042, pPoint->GetOuterRing()->GetAccuracyPoint(1)->GetLongitude(), 1E-7);
  EXPECT_NEAR(1.2340036, pPoint->GetOuterRing()->GetAccuracyPoint(2)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3450218, pPoint->GetOuterRing()->GetAccuracyPoint(2)->GetLongitude(), 1E-7);
  EAGGR_DeallocateString(handle, &pString);

  returnCode = EAGGR_ConvertDggsCellOutlineToShapeString(handle, cell, DGGS_GEO_JSON_FORMAT, &pString);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Import the string and test the values numerically because values in the string can vary slightly
  ImportExport::GeoJsonImporter geoJsonImporter(pString, 1.0e-5);
  shape = geoJsonImporter.GetNextShape();
  ASSERT_EQ(LatLong::WGS84_POLYGON, shape.GetShapeType());

  pPoint = static_cast<const LatLong::Wgs84Polygon *>(shape.GetShapeData());
  ASSERT_EQ(3, pPoint->GetOuterRing()->GetNumberOfPoints());
  EXPECT_NEAR(1.2340080, pPoint->GetOuterRing()->GetAccuracyPoint(0)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3449866, pPoint->GetOuterRing()->GetAccuracyPoint(0)->GetLongitude(), 1E-7);
  EXPECT_NEAR(1.2339790, pPoint->GetOuterRing()->GetAccuracyPoint(1)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3450042, pPoint->GetOuterRing()->GetAccuracyPoint(1)->GetLongitude(), 1E-7);
  EXPECT_NEAR(1.2340036, pPoint->GetOuterRing()->GetAccuracyPoint(2)->GetLatitude(), 1E-7);
  EXPECT_NEAR(2.3450218, pPoint->GetOuterRing()->GetAccuracyPoint(2)->GetLongitude(), 1E-7);
  EAGGR_DeallocateString(handle, &pString);

  // Test error cases
  returnCode = EAGGR_ConvertDggsCellOutlineToShapeString(NULL, cell, DGGS_WKT_FORMAT, &pString);
  EXPECT_EQ(DGGS_INVALID_HANDLE, returnCode);
  returnCode = EAGGR_ConvertDggsCellOutlineToShapeString(handle, cell, DGGS_WKT_FORMAT, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(DLL, NoLastError)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(NULL, errorMessage);
  EXPECT_EQ(0U, messageLength);
  EAGGR_DeallocateString(handle, &errorMessage);
}

SYSTEM_TEST(DLL, NullDggsHandle)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  DGGS_LatLongShape shape;
  DGGS_Shape * pOutput;
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, &shape, 1U, &pOutput);
  ASSERT_EQ(DGGS_INVALID_HANDLE, returnCode);

  // Cannot check error message because handle is not valid
}

SYSTEM_TEST(DLL, UnsupportedShape)
{
  static const unsigned short NO_OF_SHAPES = 1U;

  DGGS_LatLongLinestring linestring =
  { NULL, 0U};

  DGGS_LatLongShape latLongShapes[NO_OF_SHAPES];
  // Deliberately select a value out of the enumerated type's range
  latLongShapes[0].m_type = static_cast<DGGS_LatLongShapeType>(3);
  latLongShapes[0].m_data.m_linestring = linestring;

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_Shape * shapes;
  returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
  ASSERT_EQ(DGGS_INVALID_PARAM, returnCode);

  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(52U, messageLength);
  EXPECT_STREQ("Type of shape 1 in the input array is not supported", errorMessage);

  EAGGR_DeallocateString(handle, &errorMessage);
}

// This test has been disabled because it fails on the 64 bit build.
// The purpose of the test is to demonstrate that the correct error code is returned
// if the supplied cell Id is too long, however the test has proven that to generate
// a cell Id that is too long is convoluted so it is not considered likely that client
// code will create cell Ids of this length.
// The test has been left in for completeness - potential to revisit and fix in the
// future (ticket #106).
/*SYSTEM_TEST(DLL, DISABLED_CellIdTooLong)
 {
 static const unsigned short NO_OF_CELLS = 1U;
 static const char EXPECTED_ERROR_MSG[]
 = "EAGGR Exception: Cell ID exceeds maximum length (by 1 characters) - reduce required accuracy";

 DGGS_Handle handle = NULL;
 DGGS_ReturnCode returnCode;

 char * errorMessage;
 unsigned short messageLength = 0U;

 returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
 ASSERT_EQ(DGGS_SUCCESS, returnCode);

 // Create a DGGS_Cell which is longer than the maximum length
 // Note: This is actually quite hard to do!
 const char tooLongCellId[] = "0123012301230123012301230123012301230123012";
 DGGS_Cell cells;
 static_cast<void>(realloc(static_cast<void *>(cells), sizeof(tooLongCellId)));
 static_cast<void>(memcpy(static_cast<void *>(cells), static_cast<const void *>(tooLongCellId), sizeof(tooLongCellId)));

 DGGS_LatLongPoint points[NO_OF_CELLS];
 returnCode = EAGGR_ConvertDggsCellsToPoints(handle, &cells, NO_OF_CELLS, points);
 ASSERT_EQ(DGGS_CELL_LENGTH_TOO_LONG, returnCode);

 returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
 ASSERT_EQ(DGGS_SUCCESS, returnCode);
 EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
 EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
 EAGGR_DeallocateString(handle, &errorMessage);

 DGGS_ShapeString shapeString;
 returnCode = EAGGR_ConvertDggsCellsToShapeString(handle, &cells, NO_OF_CELLS, DGGS_WKT_FORMAT, &shapeString);
 ASSERT_EQ(DGGS_CELL_LENGTH_TOO_LONG, returnCode);

 returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
 ASSERT_EQ(DGGS_SUCCESS, returnCode);
 EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
 /// @todo Work out why this check fails on Jenkins but not my machine and then add it back to the tests.
 ///       Actual:   "EAGGR Exception: Cell ID exceeds maximum length (by 2 characters) - reduce required accuracy"
 ///       Expected: "EAGGR Exception: Cell ID exceeds maximum length (by 1 characters) - reduce required accuracy"
 //EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
 EAGGR_DeallocateString(handle, &errorMessage);

 DGGS_Cell cellArray;
 unsigned short noOfcellsInArray = 0U;
 returnCode = EAGGR_GetDggsCellParents(handle, cells, &cellArray, &noOfcellsInArray);
 ASSERT_EQ(DGGS_CELL_LENGTH_TOO_LONG, returnCode);

 returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
 ASSERT_EQ(DGGS_SUCCESS, returnCode);
 EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
 /// @todo Work out why this check fails on Jenkins but not my machine and then add it back to the tests.
 ///       Actual:   "EAGGR Exception: Cell ID exceeds maximum length (by 2 characters) - reduce required accuracy"
 ///       Expected: "EAGGR Exception: Cell ID exceeds maximum length (by 1 characters) - reduce required accuracy"
 //EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
 EAGGR_DeallocateString(handle, &errorMessage);

 returnCode = EAGGR_GetDggsCellChildren(handle, cells, &cellArray, &noOfcellsInArray);
 ASSERT_EQ(EAGGR_CELL_LENGTH_TOO_LONG, returnCode);

 returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
 ASSERT_EQ(EAGGR_SUCCESS, returnCode);
 EXPECT_EQ(sizeof(EXPECTED_ERROR_MSG), messageLength);
 /// @todo Work out why this check fails on Jenkins but not my machine and then add it back to the tests.
 ///       Actual:   "EAGGR Exception: Cell ID exceeds maximum length (by 2 characters) - reduce required accuracy"
 ///       Expected: "EAGGR Exception: Cell ID exceeds maximum length (by 1 characters) - reduce required accuracy"
 //EXPECT_STREQ(EXPECTED_ERROR_MSG, errorMessage);
 EAGGR_DeallocateString(handle, &errorMessage);
 }*/

SYSTEM_TEST(DLL, GetBoundingDggsCellISEA4T)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Define cells on a single face all at the same resolution
  DGGS_Cell cellsSameResolution[] =
  {
    "07001", "07002", "07003"
  };

  DGGS_Cell boundingCell;

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsSameResolution, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("0700", boundingCell);

  // Define cells on a single face all at different resolutions
  DGGS_Cell cellsDifferentResolutions[] =
  {
    "0700123123", "0700231231231", "07003123123123123123123"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsDifferentResolutions, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("0700", boundingCell);

  // Define cells on a single face in different initial partitions
  DGGS_Cell cellsSameFace[] =
  {
    "07123123", "07231231231", "073123123123123123123"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsSameFace, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("07", boundingCell);

  // Define cells on different faces
  DGGS_Cell cellsDifferentFaces[] =
  {
    "07001", "08002", "09003"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsDifferentFaces, 3, &boundingCell);
  ASSERT_EQ(DGGS_INVALID_PARAM, returnCode);
}

SYSTEM_TEST(DLL, GetBoundingDggsCellISEA3H)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA3H, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Define cells on a single face all at the same resolution
  DGGS_Cell cellsSameResolution[] =
  {
    "07040,0", "07040,1", "07040,-1"
  };

  DGGS_Cell boundingCell;

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsSameResolution, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("07020,0", boundingCell);

  // Define cells on a single face all at different resolutions
  DGGS_Cell cellsDifferentResolutions[] =
  {
    "07040,0", "07050,2", "0706-1,0"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsDifferentResolutions, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("07020,0", boundingCell);

  // Define cells on a single face in different initial partitions
  DGGS_Cell cellsSameFace[] =
  {
    "07030,0", "07032,0", "0703-2,-1"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsSameFace, 3, &boundingCell);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_STREQ("07000,0", boundingCell);

  // Define cells on different faces
  DGGS_Cell cellsDifferentFaces[] =
  {
    "07010,0", "08020,0", "09030,0"
  };

  returnCode = EAGGR_GetBoundingDggsCell(handle, cellsDifferentFaces, 3, &boundingCell);
  ASSERT_EQ(DGGS_INVALID_PARAM, returnCode);
}

void CheckShapeString(ImportExport::AbstractShapeImporter * a_importer)
{
  LatLong::LatLongShape shape = a_importer->GetNextShape();
  ASSERT_EQ(LatLong::WGS84_ACCURACY_POINT, shape.GetShapeType());

  const LatLong::Wgs84AccuracyPoint * pPoint =
      static_cast<const LatLong::Wgs84AccuracyPoint *>(shape.GetShapeData());
  EXPECT_NEAR(1.234, pPoint->GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(2.345, pPoint->GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(1.000e-005, pPoint->GetAccuracy(), ACCURACY_TOLERANCE);

  shape = a_importer->GetNextShape();
  ASSERT_EQ(LatLong::WGS84_ACCURACY_POINT, shape.GetShapeType());

  pPoint = static_cast<const LatLong::Wgs84AccuracyPoint *>(shape.GetShapeData());
  EXPECT_NEAR(3.456, pPoint->GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(4.567, pPoint->GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(1.000e-005, pPoint->GetAccuracy(), ACCURACY_TOLERANCE);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesEquals)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the EQUALS method

  DGGS_AnalysisType spatialAnalysisType = DGGS_EQUALS;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with itself
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_baseCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with a different shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_otherCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Compare the shape linestring with itself
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_baseLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with a different shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with itself
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_basePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with a different shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_noInnerRingsPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesContains)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the CONTAINS method

  DGGS_AnalysisType spatialAnalysisType = DGGS_CONTAINS;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the child shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_childCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with a disjoint shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Compare the shape linestring with the substring shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_substringLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the substring shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with the shape polygon containing the interior polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_interiorPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesWithin)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the WITHIN method

  DGGS_AnalysisType spatialAnalysisType = DGGS_WITHIN;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape polygon with the parent shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_parentCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the disjoint shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct a shape from a superstring of the original linestring
  DGGS_Shape superstringDggsShapeLinestring;
  superstringDggsShapeLinestring.m_type = DGGS_LINESTRING;
  superstringDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  superstringDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 5U;
  DGGS_Cell superstringLinestringCellIds[5];
  AssignCell(superstringLinestringCellIds[0], "0733");
  AssignCell(superstringLinestringCellIds[1], "0701");
  AssignCell(superstringLinestringCellIds[2], "0702");
  AssignCell(superstringLinestringCellIds[3], "0703");
  AssignCell(superstringLinestringCellIds[4], "0711");
  superstringDggsShapeLinestring.m_data.m_linestring.m_cells = superstringLinestringCellIds;

  // Compare the shape linestring with the superstring shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &superstringDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with the shape polygon that contains it within its interior
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_containsPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesTouches)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the TOUCHES method

  DGGS_AnalysisType spatialAnalysisType = DGGS_TOUCHES;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the edge sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_otherCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the disjoint shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct another shape linestring from a linestring whose end touches an end of the original linestring
  DGGS_Shape superstringDggsShapeLinestring;
  superstringDggsShapeLinestring.m_type = DGGS_LINESTRING;
  superstringDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  superstringDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell superstringLinestringCellIds[3];
  AssignCell(superstringLinestringCellIds[0], "0733");
  AssignCell(superstringLinestringCellIds[1], "0732");
  AssignCell(superstringLinestringCellIds[2], "0701");
  superstringDggsShapeLinestring.m_data.m_linestring.m_cells = superstringLinestringCellIds;

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &superstringDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Construct another shape polygon from a polygon that shares an edge with the original shape polygon
  DGGS_Shape edgeDggsShapePolygon;
  edgeDggsShapePolygon.m_type = DGGS_POLYGON;
  edgeDggsShapePolygon.m_location = DGGS_ONE_FACE;
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell edgeOuterLinestringCellIds[3];
  AssignCell(edgeOuterLinestringCellIds[0], "0702");
  AssignCell(edgeOuterLinestringCellIds[1], "0701");
  AssignCell(edgeOuterLinestringCellIds[2], "0722");
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = edgeOuterLinestringCellIds;
  edgeDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with a shape polygon that shares an edge
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &edgeDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesDisjoint)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the DISJOINT method

  DGGS_AnalysisType spatialAnalysisType = DGGS_DISJOINT;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the disjoint sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the edge shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_otherCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape linestring from a linestring whose end touches an end of the original linestring
  DGGS_Shape superstringDggsShapeLinestring;
  superstringDggsShapeLinestring.m_type = DGGS_LINESTRING;
  superstringDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  superstringDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell superstringLinestringCellIds[3];
  AssignCell(superstringLinestringCellIds[0], "0733");
  AssignCell(superstringLinestringCellIds[1], "0732");
  AssignCell(superstringLinestringCellIds[2], "0701");
  superstringDggsShapeLinestring.m_data.m_linestring.m_cells = superstringLinestringCellIds;

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &superstringDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape polygon from a polygon that shares an edge with the original shape polygon
  DGGS_Shape edgeDggsShapePolygon;
  edgeDggsShapePolygon.m_type = DGGS_POLYGON;
  edgeDggsShapePolygon.m_location = DGGS_ONE_FACE;
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell edgeOuterLinestringCellIds[3];
  AssignCell(edgeOuterLinestringCellIds[0], "0702");
  AssignCell(edgeOuterLinestringCellIds[1], "0701");
  AssignCell(edgeOuterLinestringCellIds[2], "0722");
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = edgeOuterLinestringCellIds;
  edgeDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with a shape polygon that shares an edge
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &edgeDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesIntersects)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the INTERSECTS method

  DGGS_AnalysisType spatialAnalysisType = DGGS_INTERSECTS;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the edge shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_otherCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the disjoint sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct another shape linestring from a linestring whose end touches an end of the original linestring
  DGGS_Shape superstringDggsShapeLinestring;
  superstringDggsShapeLinestring.m_type = DGGS_LINESTRING;
  superstringDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  superstringDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell superstringLinestringCellIds[3];
  AssignCell(superstringLinestringCellIds[0], "0733");
  AssignCell(superstringLinestringCellIds[1], "0732");
  AssignCell(superstringLinestringCellIds[2], "0701");
  superstringDggsShapeLinestring.m_data.m_linestring.m_cells = superstringLinestringCellIds;

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &superstringDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Construct another shape polygon from a polygon that shares an edge with the original shape polygon
  DGGS_Shape edgeDggsShapePolygon;
  edgeDggsShapePolygon.m_type = DGGS_POLYGON;
  edgeDggsShapePolygon.m_location = DGGS_ONE_FACE;
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell edgeOuterLinestringCellIds[3];
  AssignCell(edgeOuterLinestringCellIds[0], "0702");
  AssignCell(edgeOuterLinestringCellIds[1], "0701");
  AssignCell(edgeOuterLinestringCellIds[2], "0722");
  edgeDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = edgeOuterLinestringCellIds;
  edgeDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with a shape polygon that shares an edge
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &edgeDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesCovers)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the COVERS method

  DGGS_AnalysisType spatialAnalysisType = DGGS_COVERS;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the child shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_childCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the disjoint sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_substringLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with a shape polygon that lies within its interior
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_interiorPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesCoveredBy)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the COVERED_BY method

  DGGS_AnalysisType spatialAnalysisType = DGGS_COVERED_BY;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  TestData testData;

  // Test the shape cell

  // Compare the shape cell with the parent shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_parentCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape cell with the disjoint sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseCell, &testData.m_disjointCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct another shape linestring from a superstring
  DGGS_Shape superstringDggsShapeLinestring;
  superstringDggsShapeLinestring.m_type = DGGS_LINESTRING;
  superstringDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  superstringDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 5U;
  DGGS_Cell superstringLinestringCellIds[5];
  AssignCell(superstringLinestringCellIds[0], "0733");
  AssignCell(superstringLinestringCellIds[1], "0701");
  AssignCell(superstringLinestringCellIds[2], "0702");
  AssignCell(superstringLinestringCellIds[3], "0703");
  AssignCell(superstringLinestringCellIds[4], "0711");
  superstringDggsShapeLinestring.m_data.m_linestring.m_cells = superstringLinestringCellIds;

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &superstringDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_baseLinestring, &testData.m_disjointLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Compare the shape polygon with a shape polygon that contains it
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_containsPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &testData.m_basePolygon, &testData.m_disjointPolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesCrosses)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the CROSSES method

  DGGS_AnalysisType spatialAnalysisType = DGGS_CROSSES;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA3H, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Construct a shape from a cell
  DGGS_Shape dggsShapeCell;
  dggsShapeCell.m_type = DGGS_CELL;
  dggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell dggsCell = "07021,1";
  AssignCell(dggsShapeCell.m_data.m_cell, dggsCell);

  // Construct a shape from a linestring
  DGGS_Shape dggsShapeLinestring;
  dggsShapeLinestring.m_type = DGGS_LINESTRING;
  dggsShapeLinestring.m_location = DGGS_ONE_FACE;
  dggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell linestringCellIds[3];
  AssignCell(linestringCellIds[0], "07021,2");
  AssignCell(linestringCellIds[1], "07023,1");
  AssignCell(linestringCellIds[2], "07023,3");
  dggsShapeLinestring.m_data.m_linestring.m_cells = linestringCellIds;

  // Construct a shape from a polygon
  DGGS_Shape dggsShapePolygon;
  dggsShapePolygon.m_type = DGGS_POLYGON;
  dggsShapePolygon.m_location = DGGS_ONE_FACE;
  dggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 5U;
  DGGS_Cell outerLinestringCellIds[5];
  AssignCell(outerLinestringCellIds[0], "07020,2");
  AssignCell(outerLinestringCellIds[1], "07024,0");
  AssignCell(outerLinestringCellIds[2], "07024,4");
  AssignCell(outerLinestringCellIds[3], "07023,4");
  AssignCell(outerLinestringCellIds[4], "07020,3");
  dggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = outerLinestringCellIds;
  dggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 1U;
  DGGS_Cell innerRingLinestringCellIds[3];
  AssignCell(innerRingLinestringCellIds[0], "07021,2");
  AssignCell(innerRingLinestringCellIds[1], "07023,1");
  AssignCell(innerRingLinestringCellIds[2], "07023,3");
  DGGS_Linestring innerRingLinestring[1];
  innerRingLinestring[0].m_noOfCells = 3U;
  innerRingLinestring[0].m_cells = innerRingLinestringCellIds;
  dggsShapePolygon.m_data.m_polygon.m_innerRings = innerRingLinestring;

  // Test the shape cell
  // Two cells can never cross

  // Construct another shape cell from an overlapping cell
  DGGS_Shape overlapDggsShapeCell;
  overlapDggsShapeCell.m_type = DGGS_CELL;
  overlapDggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell overlapDggsCell = "07031,4";
  AssignCell(overlapDggsShapeCell.m_data.m_cell, overlapDggsCell);

  // Compare the shape cell with the overlapping shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeCell, &overlapDggsShapeCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Construct another shape cell from a disjoint cell
  DGGS_Shape disjointDggsShapeCell;
  disjointDggsShapeCell.m_type = DGGS_CELL;
  disjointDggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell disjointDggsCell = "07023,1";
  AssignCell(disjointDggsShapeCell.m_data.m_cell, disjointDggsCell);

  // Compare the shape cell with the disjoint sharing shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeCell, &disjointDggsShapeCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct another shape linestring from a linestring that crosses at a single point
  DGGS_Shape crossDggsShapeLinestring;
  crossDggsShapeLinestring.m_type = DGGS_LINESTRING;
  crossDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  crossDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell crossLinestringCellIds[3];
  AssignCell(crossLinestringCellIds[0], "07022,3");
  AssignCell(crossLinestringCellIds[1], "07023,1");
  AssignCell(crossLinestringCellIds[2], "07024,0");
  crossDggsShapeLinestring.m_data.m_linestring.m_cells = crossLinestringCellIds;

  // Compare the shape linestring with the end touching shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeLinestring, &crossDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape linestring from a linestring that is disjoint from the original linestring
  DGGS_Shape disjointDggsShapeLinestring;
  disjointDggsShapeLinestring.m_type = DGGS_LINESTRING;
  disjointDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  disjointDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 4U;
  DGGS_Cell disjointLinestringCellIds[4];
  AssignCell(disjointLinestringCellIds[0], "07020,2");
  AssignCell(disjointLinestringCellIds[1], "07023,0");
  AssignCell(disjointLinestringCellIds[2], "07024,1");
  AssignCell(disjointLinestringCellIds[3], "07024,4");
  disjointDggsShapeLinestring.m_data.m_linestring.m_cells = disjointLinestringCellIds;

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeLinestring, &disjointDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon
  // A polygon can never cross another polygon

  // Construct another shape polygon from a polygon that overlaps the original shape polygon
  DGGS_Shape overlapDggsShapePolygon;
  overlapDggsShapePolygon.m_type = DGGS_POLYGON;
  overlapDggsShapePolygon.m_location = DGGS_ONE_FACE;
  overlapDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 6U;
  DGGS_Cell overlapOuterLinestringCellIds[6];
  AssignCell(overlapOuterLinestringCellIds[0], "07020,2");
  AssignCell(overlapOuterLinestringCellIds[1], "07023,0");
  AssignCell(overlapOuterLinestringCellIds[2], "07024,1");
  AssignCell(overlapOuterLinestringCellIds[3], "07024,4");
  AssignCell(overlapOuterLinestringCellIds[4], "07023,4");
  AssignCell(overlapOuterLinestringCellIds[5], "07020,3");
  overlapDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = overlapOuterLinestringCellIds;
  overlapDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with a shape polygon that overlaps it
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapePolygon, &overlapDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Construct another shape polygon from a polygon that is disjoint from the original polygon
  DGGS_Shape disjointDggsShapePolygon;
  disjointDggsShapePolygon.m_type = DGGS_POLYGON;
  disjointDggsShapePolygon.m_location = DGGS_ONE_FACE;
  disjointDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell disjointOuterLinestringCellIds[3];
  AssignCell(disjointOuterLinestringCellIds[0], "07020,0");
  AssignCell(disjointOuterLinestringCellIds[1], "07020,1");
  AssignCell(disjointOuterLinestringCellIds[2], "07021,0");
  disjointDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = disjointOuterLinestringCellIds;
  disjointDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapePolygon, &disjointDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesOverlaps)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  // Tests the OVERLAPS method

  DGGS_AnalysisType spatialAnalysisType = DGGS_OVERLAPS;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA3H, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Construct a shape from a cell
  DGGS_Shape dggsShapeCell;
  dggsShapeCell.m_type = DGGS_CELL;
  dggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell dggsCell = "07021,1";
  AssignCell(dggsShapeCell.m_data.m_cell, dggsCell);

  // Construct a shape from a linestring
  DGGS_Shape dggsShapeLinestring;
  dggsShapeLinestring.m_type = DGGS_LINESTRING;
  dggsShapeLinestring.m_location = DGGS_ONE_FACE;
  dggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell linestringCellIds[3];
  AssignCell(linestringCellIds[0], "07021,2");
  AssignCell(linestringCellIds[1], "07023,1");
  AssignCell(linestringCellIds[2], "07023,3");
  dggsShapeLinestring.m_data.m_linestring.m_cells = linestringCellIds;

  // Construct a shape from a polygon
  DGGS_Shape dggsShapePolygon;
  dggsShapePolygon.m_type = DGGS_POLYGON;
  dggsShapePolygon.m_location = DGGS_ONE_FACE;
  dggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 5U;
  DGGS_Cell outerLinestringCellIds[5];
  AssignCell(outerLinestringCellIds[0], "07020,2");
  AssignCell(outerLinestringCellIds[1], "07024,0");
  AssignCell(outerLinestringCellIds[2], "07024,4");
  AssignCell(outerLinestringCellIds[3], "07023,4");
  AssignCell(outerLinestringCellIds[4], "07020,3");
  dggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = outerLinestringCellIds;
  dggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 1U;
  DGGS_Cell innerRingLinestringCellIds[3];
  AssignCell(innerRingLinestringCellIds[0], "07021,2");
  AssignCell(innerRingLinestringCellIds[1], "07023,1");
  AssignCell(innerRingLinestringCellIds[2], "07023,3");
  DGGS_Linestring innerRingLinestring[1];
  innerRingLinestring[0].m_noOfCells = 3U;
  innerRingLinestring[0].m_cells = innerRingLinestringCellIds;
  dggsShapePolygon.m_data.m_polygon.m_innerRings = innerRingLinestring;

  // Test the shape cell

  // Construct another shape cell from an overlapping child cell
  DGGS_Shape overlapChildDggsShapeCell;
  overlapChildDggsShapeCell.m_type = DGGS_CELL;
  overlapChildDggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell overlapChildDggsCell = "07031,4";
  AssignCell(overlapChildDggsShapeCell.m_data.m_cell, overlapChildDggsCell);

  // Compare the shape cell with the overlapping child shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeCell, &overlapChildDggsShapeCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape cell from a disjoint cell
  DGGS_Shape disjointDggsShapeCell;
  disjointDggsShapeCell.m_type = DGGS_CELL;
  disjointDggsShapeCell.m_location = DGGS_ONE_FACE;
  DGGS_Cell disjointDggsCell = "07023,1";
  AssignCell(disjointDggsShapeCell.m_data.m_cell, disjointDggsCell);

  // Compare the shape cell with the disjoint shape cell
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeCell, &disjointDggsShapeCell, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape linestring

  // Construct another shape linestring from a linestring that partially overlaps
  DGGS_Shape overlapDggsShapeLinestring;
  overlapDggsShapeLinestring.m_type = DGGS_LINESTRING;
  overlapDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  overlapDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 3U;
  DGGS_Cell overlapLinestringCellIds[3];
  AssignCell(overlapLinestringCellIds[0], "07021,4");
  AssignCell(overlapLinestringCellIds[1], "07021,2");
  AssignCell(overlapLinestringCellIds[2], "07023,1");
  overlapDggsShapeLinestring.m_data.m_linestring.m_cells = overlapLinestringCellIds;

  // Compare the shape linestring with a shape linestring that partially overlaps
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeLinestring, &overlapDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape linestring from a linestring that is disjoint from the original linestring
  DGGS_Shape disjointDggsShapeLinestring;
  disjointDggsShapeLinestring.m_type = DGGS_LINESTRING;
  disjointDggsShapeLinestring.m_location = DGGS_ONE_FACE;
  disjointDggsShapeLinestring.m_data.m_linestring.m_noOfCells = 4U;
  DGGS_Cell disjointLinestringCellIds[4];
  AssignCell(disjointLinestringCellIds[0], "07020,2");
  AssignCell(disjointLinestringCellIds[1], "07023,0");
  AssignCell(disjointLinestringCellIds[2], "07024,1");
  AssignCell(disjointLinestringCellIds[3], "07024,4");
  disjointDggsShapeLinestring.m_data.m_linestring.m_cells = disjointLinestringCellIds;

  // Compare the shape linestring with the disjoint shape linestring
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapeLinestring, &disjointDggsShapeLinestring, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);

  // Test the shape polygon

  // Construct another shape polygon from a polygon that overlaps the original shape polygon
  DGGS_Shape overlapDggsShapePolygon;
  overlapDggsShapePolygon.m_type = DGGS_POLYGON;
  overlapDggsShapePolygon.m_location = DGGS_ONE_FACE;
  overlapDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell overlapOuterLinestringCellIds[3];
  AssignCell(overlapOuterLinestringCellIds[0], "07032,3");
  AssignCell(overlapOuterLinestringCellIds[1], "07023,4");
  AssignCell(overlapOuterLinestringCellIds[2], "07022,3");
  overlapDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = overlapOuterLinestringCellIds;
  overlapDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with a shape polygon that overlaps it
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapePolygon, &overlapDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_TRUE(shapeComparisonResult);

  // Construct another shape polygon from a polygon that is disjoint from the original polygon
  DGGS_Shape disjointDggsShapePolygon;
  disjointDggsShapePolygon.m_type = DGGS_POLYGON;
  disjointDggsShapePolygon.m_location = DGGS_ONE_FACE;
  disjointDggsShapePolygon.m_data.m_polygon.m_outerRing.m_noOfCells = 3U;
  DGGS_Cell disjointOuterLinestringCellIds[3];
  AssignCell(disjointOuterLinestringCellIds[0], "07020,0");
  AssignCell(disjointOuterLinestringCellIds[1], "07020,1");
  AssignCell(disjointOuterLinestringCellIds[2], "07021,0");
  disjointDggsShapePolygon.m_data.m_polygon.m_outerRing.m_cells = disjointOuterLinestringCellIds;
  disjointDggsShapePolygon.m_data.m_polygon.m_noOfInnerRings = 0U;

  // Compare the shape polygon with the disjoint shape polygon
  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &dggsShapePolygon, &disjointDggsShapePolygon, &shapeComparisonResult);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
  EXPECT_FALSE(shapeComparisonResult);
}

SYSTEM_TEST(DLL, EAGGR_CompareShapesErrors)
{
  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;
  bool shapeComparisonResult;

  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  DGGS_AnalysisType spatialAnalysisType = DGGS_EQUALS;

  DGGS_Shape validShape;
  validShape.m_type = DGGS_CELL;
  validShape.m_location = DGGS_ONE_FACE;
  DGGS_Cell dggsCell = "0700";
  AssignCell(validShape.m_data.m_cell, dggsCell);

  DGGS_Shape invalidShape;
  invalidShape.m_type = (DGGS_ShapeType)100; // This is invalid
  invalidShape.m_location = DGGS_ONE_FACE;
  AssignCell(invalidShape.m_data.m_cell, dggsCell);

  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &validShape, &validShape, NULL);
  EXPECT_EQ(DGGS_NULL_POINTER, returnCode);

  char * errorMessage;
  unsigned short messageLength = 0U;
  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(51U, messageLength);
  EXPECT_STREQ("Pointer argument 'a_shapeComparisonResult' is null", errorMessage);

  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &validShape, &invalidShape, &shapeComparisonResult);
  EXPECT_EQ(DGGS_MODEL_ERROR, returnCode);

  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(52U, messageLength);
  EXPECT_STREQ("EAGGR Exception: Unsupported comparison shape type.", errorMessage);

  EAGGR_DeallocateString(handle, &errorMessage);

  returnCode = EAGGR_CompareShapes(handle, spatialAnalysisType, &invalidShape, &validShape, &shapeComparisonResult);
  EXPECT_EQ(DGGS_MODEL_ERROR, returnCode);

  returnCode = EAGGR_GetLastErrorMessage(handle, &errorMessage, &messageLength);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  EXPECT_EQ(46U, messageLength);
  EXPECT_STREQ("EAGGR Exception: Unsupported base shape type.", errorMessage);

  EAGGR_DeallocateString(handle, &errorMessage);
}
