//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file GeoJsonExporter.cpp
///
/// Tests for the EAGGR::ImportExport::GeoJsonExporter class.
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

#include "Src/ImportExport/GeoJsonExporter.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::ImportExport;
using namespace EAGGR::LatLong;

UNIT_TEST(GeoJsonExporter, ExportPoint)
{
  Wgs84AccuracyPoint point(1.234, 2.345, 0.0001);

  std::vector<LatLongShape> shapes;
  shapes.push_back(LatLongShape(WGS84_ACCURACY_POINT, &point));

  GeoJsonExporter exporter;

  std::string geoJsonString = exporter.ExportShapes(shapes);

  std::string expectedGeoJsonString = "{ \"type\": \"Point\", \"coordinates\": [ 2.345, 1.234 ] }";

  EXPECT_EQ(expectedGeoJsonString, geoJsonString);
}

UNIT_TEST(GeoJsonExporter, ExportMultiPoint)
{
  Wgs84AccuracyPoint point1(1.234, 2.345, 0.0001);
  Wgs84AccuracyPoint point2(3.456, 4.567, 0.0001);

  std::vector<LatLongShape> shapes;
  shapes.push_back(LatLongShape(WGS84_ACCURACY_POINT, &point1));
  shapes.push_back(LatLongShape(WGS84_ACCURACY_POINT, &point2));

  GeoJsonExporter exporter;

  std::string geoJsonString = exporter.ExportShapes(shapes);

  std::string expectedGeoJsonString = "{ \"type\": \"MultiPoint\", \"coordinates\": [ [ 2.345, 1.234 ], [ 4.567, 3.456 ] ] }";

  EXPECT_EQ(expectedGeoJsonString, geoJsonString);
}

UNIT_TEST(GeoJsonExporter, ExportPolygon)
{
  Wgs84Polygon polygon;
  polygon.AddAccuracyPointToOuterRing(1.234, 2.345, 0.0001);
  polygon.AddAccuracyPointToOuterRing(3.456, 4.567, 0.0001);

  std::vector<LatLongShape> shapes;
  shapes.push_back(LatLongShape(WGS84_POLYGON, &polygon));

  GeoJsonExporter exporter;

  std::string geoJsonString = exporter.ExportShapes(shapes);

  std::string expectedGeoJsonString =
  "{ "
  "\"type\": \"Polygon\", "
  "\"coordinates\": [ [ [ 2.345, 1.234 ], [ 4.567, 3.456 ] ] ] "
  "}";

  EXPECT_EQ(expectedGeoJsonString, geoJsonString);
}

UNIT_TEST(GeoJsonExporter, ExportPolygonWithInnerRing)
{
  Wgs84Polygon polygon;
  polygon.AddAccuracyPointToOuterRing(1.234, 2.345, 0.0001);
  polygon.AddAccuracyPointToOuterRing(3.456, 4.567, 0.0001);

  polygon.CreateInnerRing();

  std::vector<LatLongShape> shapes;
  shapes.push_back(LatLongShape(WGS84_POLYGON, &polygon));

  GeoJsonExporter exporter;

  EXPECT_THROW(exporter.ExportShapes(shapes), EAGGR::EAGGRException);
}

UNIT_TEST(GeoJsonExporter, ExportMultiPolygon)
{
  Wgs84Polygon polygon1;
  polygon1.AddAccuracyPointToOuterRing(1.234, 2.345, 0.0001);
  polygon1.AddAccuracyPointToOuterRing(3.456, 4.567, 0.0001);

  Wgs84Polygon polygon2;
  polygon2.AddAccuracyPointToOuterRing(1.234, 2.345, 0.0001);
  polygon2.AddAccuracyPointToOuterRing(3.456, 4.567, 0.0001);

  std::vector<LatLongShape> shapes;
  shapes.push_back(LatLongShape(WGS84_POLYGON, &polygon1));
  shapes.push_back(LatLongShape(WGS84_POLYGON, &polygon2));

  GeoJsonExporter exporter;

  EXPECT_THROW(exporter.ExportShapes(shapes), EAGGR::EAGGRException);
}
