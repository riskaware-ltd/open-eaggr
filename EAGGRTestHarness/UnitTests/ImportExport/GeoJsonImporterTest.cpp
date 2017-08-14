//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file GeoJsonImporterTest.cpp
///
/// Tests for the EAGGR::ImportExport::GeoJsonImporter class.
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

#include "Src/EAGGRException.hpp"
#include "Src/ImportExport/GeoJsonImporter.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/LatLong/Wgs84Linestring.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"

using namespace EAGGR::ImportExport;
using namespace EAGGR::LatLong;

UNIT_TEST(GeoJsonImporter, ImportSimplePoint)
{
  const std::string geoJson = "{ \"type\": \"Point\", \"coordinates\": [1.234, 2.345] }";
  double accuracy = 0.0001;

  GeoJsonImporter importer(geoJson, accuracy);

  ASSERT_TRUE(importer.HasNext());

  LatLongShape shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  const Wgs84AccuracyPoint* point = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(2.345, point->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, point->GetLongitude());
  EXPECT_DOUBLE_EQ(accuracy, point->GetAccuracy());

  EXPECT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportMultiPoint)
{
  const std::string geoJson = "{ \"type\": \"MultiPoint\", \"coordinates\": [ [1.234, 2.345], [3.456, 4.567] ] }";
  double accuracy = 0.0001;

  GeoJsonImporter importer(geoJson, accuracy);

  ASSERT_TRUE(importer.HasNext());

  LatLongShape shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  const Wgs84AccuracyPoint* point = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(2.345, point->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, point->GetLongitude());
  EXPECT_DOUBLE_EQ(accuracy, point->GetAccuracy());

  ASSERT_TRUE(importer.HasNext());

  shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  point = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(4.567, point->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, point->GetLongitude());
  EXPECT_DOUBLE_EQ(accuracy, point->GetAccuracy());

  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportSimpleLinestring)
{
  const std::string geoJson =
  "{ "
  "\"type\": \"LineString\", "
  "\"coordinates\": [[1.234, 2.345], [3.456, 4.567]] "
  "}";
  constexpr double ACCURACY = 0.0001;
  GeoJsonImporter importer(geoJson, ACCURACY);

  // Get the shape
  ASSERT_TRUE(importer.HasNext());
  LatLongShape shape = importer.GetNextShape();

  // Check the shape is a linestring
  ASSERT_EQ(WGS84_LINESTRING, shape.GetShapeType());
  const Wgs84Linestring* pLinestring = static_cast<const Wgs84Linestring*>(shape.GetShapeData());

  // Check the first point of the linestring
  unsigned short pointIndex = 0U;
  const Wgs84AccuracyPoint* pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check the second point of the linestring
  pointIndex++;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the linestring
  pointIndex++;
  ASSERT_THROW(pLinestring->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no more shapes
  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportMultiLinestring)
{
  const std::string geoJson =
  "{ \"type\": \"MultiLineString\", "
  "\"coordinates\": [[[1.234, 2.345], [3.456, 4.567]], [[5.678, 6.789], [7.890, 8.901]]] }";
  constexpr double ACCURACY(0.0001);
  GeoJsonImporter importer(geoJson, ACCURACY);

  // Get the first shape
  ASSERT_TRUE(importer.HasNext());
  LatLongShape shape = importer.GetNextShape();

  // Check the shape is a linestring
  ASSERT_EQ(WGS84_LINESTRING, shape.GetShapeType());
  const Wgs84Linestring* pLinestring = static_cast<const Wgs84Linestring*>(shape.GetShapeData());

  // Check the first point of the linestring
  unsigned short pointIndex = 0U;
  const Wgs84AccuracyPoint* pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check the second point of the linestring
  pointIndex++;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the linestring
  pointIndex++;
  ASSERT_THROW(pLinestring->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Get the second shape
  ASSERT_TRUE(importer.HasNext());
  shape = importer.GetNextShape();

  // Check the shape is a linestring
  ASSERT_EQ(WGS84_LINESTRING, shape.GetShapeType());
  pLinestring = static_cast<const Wgs84Linestring*>(shape.GetShapeData());

  // Check the first point of the linestring
  pointIndex = 0U;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(6.789, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(5.678, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check the second point of the linestring
  pointIndex++;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(8.901, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(7.890, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the linestring
  pointIndex++;
  ASSERT_THROW(pLinestring->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no more shapes
  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportSimplePolygon)
{
  const std::string geoJson =
  "{ "
  "\"type\": \"Polygon\", "
  "\"coordinates\": [[[1.234, 2.345], [3.456, 4.567]], [[5.678, 6.789]], [[7.890, 8.901]]] "
  "}";
  constexpr double ACCURACY = 0.0001;
  GeoJsonImporter importer(geoJson, ACCURACY);

  // Get the shape
  ASSERT_TRUE(importer.HasNext());
  LatLongShape shape = importer.GetNextShape();

  // Check the shape is a polygon
  ASSERT_EQ(WGS84_POLYGON, shape.GetShapeType());
  const Wgs84Polygon* pPolygon = static_cast<const Wgs84Polygon*>(shape.GetShapeData());

  // Check the outer ring of the polygon
  const Wgs84Linestring* pOuterRing = pPolygon->GetOuterRing();
  ASSERT_EQ(2U, pOuterRing->GetNumberOfPoints());

  unsigned short pointIndex = 0U;
  const Wgs84AccuracyPoint* pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  pointIndex++;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the outer ring
  pointIndex++;
  ASSERT_THROW(pOuterRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check the number of inner rings in the polygon
  ASSERT_EQ(2U, pPolygon->GetNumberOfInnerRings());

  // Check the first inner ring of the polygon
  const Wgs84Linestring* pInnerRing = pPolygon->GetInnerRing(0U);
  ASSERT_EQ(1U, pInnerRing->GetNumberOfPoints());

  pointIndex = 0U;
  pPoint = pInnerRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(6.789, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(5.678, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the first inner ring
  pointIndex++;
  ASSERT_THROW(pInnerRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check the second inner ring of the polygon
  pInnerRing = pPolygon->GetInnerRing(1U);
  ASSERT_EQ(1U, pInnerRing->GetNumberOfPoints());

  pointIndex = 0U;
  pPoint = pInnerRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(8.901, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(7.890, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the second inner ring
  pointIndex++;
  ASSERT_THROW(pInnerRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no more shapes
  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportMultiPolygon)
{
  const std::string geoJson =
  "{ "
  "\"type\": \"MultiPolygon\", "
  "\"coordinates\": [[[[1.234, 2.345], [3.456, 4.567]]], [[[5.678, 6.789], [7.890, 8.901]]]] "
  "}";
  constexpr double ACCURACY(0.0001);
  GeoJsonImporter importer(geoJson, ACCURACY);

  // Get the first shape
  ASSERT_TRUE(importer.HasNext());
  LatLongShape shape = importer.GetNextShape();

  // Check the first shape is a polygon
  ASSERT_EQ(WGS84_POLYGON, shape.GetShapeType());
  const Wgs84Polygon* pPolygon = static_cast<const Wgs84Polygon*>(shape.GetShapeData());

  // Check the outer ring of the first polygon
  const Wgs84Linestring* pOuterRing = pPolygon->GetOuterRing();
  ASSERT_EQ(2U, pOuterRing->GetNumberOfPoints());

  unsigned short pointIndex = 0U;
  const Wgs84AccuracyPoint* pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  pointIndex++;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the outer ring
  pointIndex++;
  ASSERT_THROW(pOuterRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no inner rings in the polygon
  ASSERT_EQ(0U, pPolygon->GetNumberOfInnerRings());

  // Get the second shape
  ASSERT_TRUE(importer.HasNext());
  shape = importer.GetNextShape();

  // Check the second shape is also a polygon
  ASSERT_EQ(WGS84_POLYGON, shape.GetShapeType());
  pPolygon = static_cast<const Wgs84Polygon*>(shape.GetShapeData());

  // Check the outer ring of the second polygon
  pOuterRing = pPolygon->GetOuterRing();
  ASSERT_EQ(2U, pOuterRing->GetNumberOfPoints());

  pointIndex = 0U;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(6.789, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(5.678, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  pointIndex++;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(8.901, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(7.890, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the outer ring
  pointIndex++;
  ASSERT_THROW(pOuterRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no inner rings in the polygon
  ASSERT_EQ(0U, pPolygon->GetNumberOfInnerRings());

  // Check there are no more shapes
  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, ImportGeometryCollection)
{
  const std::string geoJson =
  "{ \"type\": \"GeometryCollection\", "
  "\"geometries\": [ "
  "{ \"type\": \"Point\", \"coordinates\": [1.234, 2.345] }, "
  "{ \"type\": \"MultiPoint\", \"coordinates\": [ [3.456, 4.567], [5.678, 6.789] ] }, "
  "{ \"type\": \"LineString\", \"coordinates\": [ [7.890, 8.901], [9.012, 0.123] ] }, "
  "{ \"type\": \"Polygon\", \"coordinates\": [ [ [1.234, 2.345], [3.456, 4.567] ] ] } ] }";
  constexpr double ACCURACY = 0.0001;

  GeoJsonImporter importer(geoJson, ACCURACY);

  ASSERT_TRUE(importer.HasNext());

  LatLongShape shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  const Wgs84AccuracyPoint* pPoint = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  ASSERT_TRUE(importer.HasNext());

  shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  pPoint = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  ASSERT_TRUE(importer.HasNext());

  shape = importer.GetNextShape();

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());

  pPoint = static_cast<const Wgs84AccuracyPoint*>(shape.GetShapeData());
  EXPECT_DOUBLE_EQ(6.789, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(5.678, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Get the third shape
  ASSERT_TRUE(importer.HasNext());
  shape = importer.GetNextShape();

  // Check the shape is a linestring
  ASSERT_EQ(WGS84_LINESTRING, shape.GetShapeType());
  const Wgs84Linestring* pLinestring = static_cast<const Wgs84Linestring*>(shape.GetShapeData());

  // Check the first point of the linestring
  unsigned short pointIndex = 0U;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(8.901, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(7.890, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check the second point of the linestring
  pointIndex++;
  pPoint = pLinestring->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(0.123, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(9.012, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the linestring
  pointIndex++;
  ASSERT_THROW(pLinestring->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Get the fourth shape
  ASSERT_TRUE(importer.HasNext());
  shape = importer.GetNextShape();

  // Check the shape is a polygon
  ASSERT_EQ(WGS84_POLYGON, shape.GetShapeType());
  const Wgs84Polygon* pPolygon = static_cast<const Wgs84Polygon*>(shape.GetShapeData());

  // Check the outer ring of the polygon
  const Wgs84Linestring* pOuterRing = pPolygon->GetOuterRing();
  ASSERT_EQ(2U, pOuterRing->GetNumberOfPoints());

  pointIndex = 0U;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(2.345, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(1.234, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  pointIndex++;
  pPoint = pOuterRing->GetAccuracyPoint(pointIndex);
  EXPECT_DOUBLE_EQ(4.567, pPoint->GetLatitude());
  EXPECT_DOUBLE_EQ(3.456, pPoint->GetLongitude());
  EXPECT_DOUBLE_EQ(ACCURACY, pPoint->GetAccuracy());

  // Check there are no more points in the outer ring
  pointIndex++;
  ASSERT_THROW(pOuterRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check there are no inner rings in the polygon
  ASSERT_EQ(0U, pPolygon->GetNumberOfInnerRings());

  // Check there are no more shapes
  ASSERT_FALSE(importer.HasNext());
}

UNIT_TEST(GeoJsonImporter, InvalidGeoJSON)
{
  // Too many arguments in POINT declaration
  const std::string geoJson = "{ \"type\": \"Point\", \"coordinates\": [1.234, 2.345, 3.456 4.567] }";
  double accuracy = 0.0001;

  EXPECT_THROW(GeoJsonImporter(geoJson, accuracy), EAGGR::EAGGRException);
}

UNIT_TEST(GeoJsonImporter, InvalidGeometry)
{
  // All GeoJSON geometry objects are supported so try one that does not exist
  const std::string geoJson = "{ \"type\": \"InvalidGeometry\", \"coordinates\": [ [4e6, -2e6], [8e6, 2e6] ] }";
  double accuracy = 0.0001;

  EXPECT_THROW(GeoJsonImporter(geoJson, accuracy), EAGGR::EAGGRException);
}
