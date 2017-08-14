//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file LinestringAnalyserTest.cpp
/// 
/// Tests for the EAGGR::SpatialAnalysis::LinestringAnalyser class.
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

#include "Src/SpatialAnalysis/LinestringAnalyser.hpp"

using namespace EAGGR::SpatialAnalysis;

UNIT_TEST(LinestringAnalyser, Equals)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointGeometry;
  pointGeometry.m_type = GeometryType::POINT;
  pointGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry reverseLineGeometry;
  reverseLineGeometry.m_type = GeometryType::LINESTRING;
  reverseLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));
  reverseLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry differentLineGeometry;
  differentLineGeometry.m_type = GeometryType::LINESTRING;
  differentLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));
  differentLineGeometry.m_linestring.push_back(point_type(5.0, 6.0));

  Geometry polygonGeometry;
  polygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerLine;
  outerLine.push_back(point_type(3.0, 4.0));
  outerLine.push_back(point_type(4.0, 5.0));
  outerLine.push_back(point_type(0.0, 6.0));
  outerLine.push_back(point_type(3.0, 4.0));
  boost::geometry::assign_points(polygonGeometry.m_polygon.outer(), outerLine);

  EXPECT_FALSE(analyser.Equals(pointGeometry));
  EXPECT_TRUE(analyser.Equals(sameLineGeometry));
  EXPECT_TRUE(analyser.Equals(reverseLineGeometry));
  EXPECT_FALSE(analyser.Equals(differentLineGeometry));
  EXPECT_FALSE(analyser.Equals(polygonGeometry));
}

UNIT_TEST(LinestringAnalyser, Intersects)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_TRUE(analyser.Intersects(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Intersects(pointOffLineGeometry));
  EXPECT_TRUE(analyser.Intersects(sameLineGeometry));
  EXPECT_TRUE(analyser.Intersects(boundaryLineGeometry));
  EXPECT_TRUE(analyser.Intersects(crossingLineGeometry));
  EXPECT_FALSE(analyser.Intersects(disjointLineGeometry));
  EXPECT_TRUE(analyser.Intersects(polygonGeometryWithLineInside));
  EXPECT_TRUE(analyser.Intersects(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Intersects(polygonGeometryWithLineOutside));

  // Also test disjoint - disjoint is !intersects
  EXPECT_FALSE(analyser.Disjoint(pointOnLineGeometry));
  EXPECT_TRUE(analyser.Disjoint(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(sameLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(crossingLineGeometry));
  EXPECT_TRUE(analyser.Disjoint(disjointLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Disjoint(polygonGeometryWithLineOnEdge));
  EXPECT_TRUE(analyser.Disjoint(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, Touches)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_TRUE(analyser.Touches(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Touches(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Touches(sameLineGeometry));
  EXPECT_FALSE(analyser.Touches(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Touches(crossingLineGeometry));
  EXPECT_FALSE(analyser.Touches(disjointLineGeometry));
  EXPECT_TRUE(analyser.Touches(touchingLineGeometry));
  EXPECT_FALSE(analyser.Touches(polygonGeometryWithLineInside));
  EXPECT_TRUE(analyser.Touches(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Touches(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, Contains)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_TRUE(analyser.Contains(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Contains(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Contains(pointOnEndOfLineGeometry));
  EXPECT_TRUE(analyser.Contains(sameLineGeometry));
  EXPECT_TRUE(analyser.Contains(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Contains(crossingLineGeometry));
  EXPECT_FALSE(analyser.Contains(disjointLineGeometry));
  EXPECT_FALSE(analyser.Contains(touchingLineGeometry));
  EXPECT_FALSE(analyser.Contains(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Contains(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Contains(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, Covers)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_TRUE(analyser.Covers(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Covers(pointOffLineGeometry));
  EXPECT_TRUE(analyser.Covers(pointOnEndOfLineGeometry));
  EXPECT_TRUE(analyser.Covers(sameLineGeometry));
  EXPECT_TRUE(analyser.Covers(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Covers(crossingLineGeometry));
  EXPECT_FALSE(analyser.Covers(disjointLineGeometry));
  EXPECT_FALSE(analyser.Covers(touchingLineGeometry));
  EXPECT_FALSE(analyser.Covers(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Covers(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Covers(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, Within)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_FALSE(analyser.Within(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Within(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Within(pointOnEndOfLineGeometry));
  EXPECT_TRUE(analyser.Within(sameLineGeometry));
  EXPECT_FALSE(analyser.Within(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Within(crossingLineGeometry));
  EXPECT_FALSE(analyser.Within(disjointLineGeometry));
  EXPECT_FALSE(analyser.Within(touchingLineGeometry));
  EXPECT_TRUE(analyser.Within(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Within(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Within(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, CoveredBy)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  EXPECT_FALSE(analyser.CoveredBy(pointOnLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(pointOffLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(pointOnEndOfLineGeometry));
  EXPECT_TRUE(analyser.CoveredBy(sameLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(boundaryLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(crossingLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(disjointLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(touchingLineGeometry));
  EXPECT_TRUE(analyser.CoveredBy(polygonGeometryWithLineInside));
  EXPECT_TRUE(analyser.CoveredBy(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.CoveredBy(polygonGeometryWithLineOutside));
}

UNIT_TEST(LinestringAnalyser, Crosses)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  Geometry polygonGeometryWithLineCrossing;
  polygonGeometryWithLineCrossing.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineCrossing;
  outerRingLineCrossing.push_back(point_type(3.25, 3.25));
  outerRingLineCrossing.push_back(point_type(3.25, 4.5));
  outerRingLineCrossing.push_back(point_type(4.5, 4.5));
  outerRingLineCrossing.push_back(point_type(4.5, 3.25));
  outerRingLineCrossing.push_back(point_type(3.25, 3.25));
  boost::geometry::assign_points(polygonGeometryWithLineCrossing.m_polygon.outer(), outerRingLineCrossing);

  Geometry polygonGeometryWithLinePartCrossing;
  polygonGeometryWithLinePartCrossing.m_type = GeometryType::POLYGON;
  linestring_type outerRingLinePartCrossing;
  outerRingLinePartCrossing.push_back(point_type(3.25, 3.25));
  outerRingLinePartCrossing.push_back(point_type(3.25, 5.5));
  outerRingLinePartCrossing.push_back(point_type(5.5, 5.5));
  outerRingLinePartCrossing.push_back(point_type(5.5, 3.25));
  outerRingLinePartCrossing.push_back(point_type(3.25, 3.25));
  boost::geometry::assign_points(polygonGeometryWithLinePartCrossing.m_polygon.outer(), outerRingLinePartCrossing);

  EXPECT_FALSE(analyser.Crosses(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Crosses(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Crosses(pointOnEndOfLineGeometry));
  EXPECT_FALSE(analyser.Crosses(sameLineGeometry));
  EXPECT_FALSE(analyser.Crosses(boundaryLineGeometry));
  EXPECT_TRUE(analyser.Crosses(crossingLineGeometry));
  EXPECT_FALSE(analyser.Crosses(disjointLineGeometry));
  EXPECT_FALSE(analyser.Crosses(touchingLineGeometry));
  EXPECT_FALSE(analyser.Crosses(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Crosses(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Crosses(polygonGeometryWithLineOutside));
  EXPECT_TRUE(analyser.Crosses(polygonGeometryWithLineCrossing));
  EXPECT_TRUE(analyser.Crosses(polygonGeometryWithLinePartCrossing));
}

UNIT_TEST(LinestringAnalyser, Overlaps)
{
  linestring_type linestring;
  linestring.push_back(point_type(3.0, 4.0));
  linestring.push_back(point_type(4.0, 5.0));

  LinestringAnalyser analyser(linestring);

  // Define geometries to compare to the initial geometry
  Geometry pointOnLineGeometry;
  pointOnLineGeometry.m_type = GeometryType::POINT;
  pointOnLineGeometry.m_point = point_type(3.5, 4.5);

  Geometry pointOffLineGeometry;
  pointOffLineGeometry.m_type = GeometryType::POINT;
  pointOffLineGeometry.m_point = point_type(0.0, 0.0);

  Geometry pointOnEndOfLineGeometry;
  pointOnEndOfLineGeometry.m_type = GeometryType::POINT;
  pointOnEndOfLineGeometry.m_point = point_type(3.0, 4.0);

  Geometry sameLineGeometry;
  sameLineGeometry.m_type = GeometryType::LINESTRING;
  sameLineGeometry.m_linestring.push_back(point_type(2.0, 3.0));
  sameLineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(2.0, 3.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 4.5));
  boundaryLineGeometry.m_linestring.push_back(point_type(3.5, 5.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  crossingLineGeometry.m_linestring.push_back(point_type(4.0, 4.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(0.0, 0.0));
  disjointLineGeometry.m_linestring.push_back(point_type(1.0, 1.0));

  Geometry touchingLineGeometry;
  touchingLineGeometry.m_type = GeometryType::LINESTRING;
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 5.0));
  touchingLineGeometry.m_linestring.push_back(point_type(3.0, 4.0));

  Geometry polygonGeometryWithLineInside;
  polygonGeometryWithLineInside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineInside;
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 5.0));
  outerRingLineInside.push_back(point_type(5.0, 0.0));
  outerRingLineInside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineInside.m_polygon.outer(), outerRingLineInside);

  Geometry polygonGeometryWithLineOnEdge;
  polygonGeometryWithLineOnEdge.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOnEdge;
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  outerRingLineOnEdge.push_back(point_type(3.0, 4.0));
  outerRingLineOnEdge.push_back(point_type(4.0, 5.0));
  outerRingLineOnEdge.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOnEdge.m_polygon.outer(), outerRingLineOnEdge);

  Geometry polygonGeometryWithLineOutside;
  polygonGeometryWithLineOutside.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineOutside;
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  outerRingLineOutside.push_back(point_type(1.0, 0.0));
  outerRingLineOutside.push_back(point_type(0.0, 1.0));
  outerRingLineOutside.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygonGeometryWithLineOutside.m_polygon.outer(), outerRingLineOutside);

  Geometry polygonGeometryWithLineCrossing;
  polygonGeometryWithLineCrossing.m_type = GeometryType::POLYGON;
  linestring_type outerRingLineCrossing;
  outerRingLineCrossing.push_back(point_type(3.25, 3.25));
  outerRingLineCrossing.push_back(point_type(3.25, 4.5));
  outerRingLineCrossing.push_back(point_type(4.5, 4.5));
  outerRingLineCrossing.push_back(point_type(4.5, 3.25));
  outerRingLineCrossing.push_back(point_type(3.25, 3.25));
  boost::geometry::assign_points(polygonGeometryWithLineCrossing.m_polygon.outer(), outerRingLineCrossing);

  Geometry polygonGeometryWithLinePartCrossing;
  polygonGeometryWithLinePartCrossing.m_type = GeometryType::POLYGON;
  linestring_type outerRingLinePartCrossing;
  outerRingLinePartCrossing.push_back(point_type(3.25, 3.25));
  outerRingLinePartCrossing.push_back(point_type(3.25, 5.5));
  outerRingLinePartCrossing.push_back(point_type(5.5, 5.5));
  outerRingLinePartCrossing.push_back(point_type(5.5, 3.25));
  outerRingLinePartCrossing.push_back(point_type(3.25, 3.25));
  boost::geometry::assign_points(polygonGeometryWithLinePartCrossing.m_polygon.outer(), outerRingLinePartCrossing);

  EXPECT_FALSE(analyser.Overlaps(pointOnLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(pointOffLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(pointOnEndOfLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(sameLineGeometry));
  EXPECT_TRUE(analyser.Overlaps(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(crossingLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(disjointLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(touchingLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(polygonGeometryWithLineInside));
  EXPECT_FALSE(analyser.Overlaps(polygonGeometryWithLineOnEdge));
  EXPECT_FALSE(analyser.Overlaps(polygonGeometryWithLineOutside));
  EXPECT_FALSE(analyser.Overlaps(polygonGeometryWithLineCrossing));
  EXPECT_FALSE(analyser.Overlaps(polygonGeometryWithLinePartCrossing));
}
