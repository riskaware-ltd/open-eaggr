//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file PolygonAnalyserTest.cpp
/// 
/// Tests for the EAGGR::SpatialAnalysis::PolygonAnalyser class.
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

#include "Src/SpatialAnalysis/PolygonAnalyser.hpp"

using namespace EAGGR::SpatialAnalysis;

UNIT_TEST(PolygonAnalyser, Equals)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointGeometry;
  pointGeometry.m_type = GeometryType::POINT;
  pointGeometry.m_point = point_type(3.0, 4.0);

  Geometry lineGeometry;
  lineGeometry.m_type = GeometryType::LINESTRING;
  lineGeometry.m_linestring.push_back(point_type(3.0, 4.0));
  lineGeometry.m_linestring.push_back(point_type(4.0, 5.0));

  Geometry samePolygon;
  samePolygon.m_type = POLYGON;
  samePolygon.m_polygon = polygon;

  Geometry otherPolygonGeometry;
  otherPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type otherPolygonOuterLine;
  otherPolygonOuterLine.push_back(point_type(3.0, 4.0));
  otherPolygonOuterLine.push_back(point_type(4.0, 5.0));
  otherPolygonOuterLine.push_back(point_type(0.0, 6.0));
  otherPolygonOuterLine.push_back(point_type(3.0, 4.0));
  boost::geometry::assign_points(otherPolygonGeometry.m_polygon.outer(), otherPolygonOuterLine);

  EXPECT_FALSE(analyser.Equals(pointGeometry));
  EXPECT_FALSE(analyser.Equals(lineGeometry));
  EXPECT_TRUE(analyser.Equals(samePolygon));
  EXPECT_FALSE(analyser.Equals(otherPolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Intersects)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry pointInPolygonHole;
  pointInPolygonHole.m_type = GeometryType::POINT;
  pointInPolygonHole.m_point = point_type(1.5, 1.5);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  EXPECT_TRUE(analyser.Intersects(pointOnPolygonEdgeGeometry));
  EXPECT_TRUE(analyser.Intersects(pointInsidePolygon));
  EXPECT_FALSE(analyser.Intersects(pointOutsidePolygon));
  EXPECT_FALSE(analyser.Intersects(pointInPolygonHole));
  EXPECT_TRUE(analyser.Intersects(boundaryLineGeometry));
  EXPECT_TRUE(analyser.Intersects(crossingLineGeometry));
  EXPECT_FALSE(analyser.Intersects(disjointLineGeometry));
  EXPECT_TRUE(analyser.Intersects(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Intersects(noIntersectPolygonGeometry));

  // Also test disjoint - disjoint is !intersects
  EXPECT_FALSE(analyser.Disjoint(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.Disjoint(pointInsidePolygon));
  EXPECT_TRUE(analyser.Disjoint(pointOutsidePolygon));
  EXPECT_TRUE(analyser.Disjoint(pointInPolygonHole));
  EXPECT_FALSE(analyser.Disjoint(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(crossingLineGeometry));
  EXPECT_TRUE(analyser.Disjoint(disjointLineGeometry));
  EXPECT_FALSE(analyser.Disjoint(partialIntersectPolygonGeometry));
  EXPECT_TRUE(analyser.Disjoint(noIntersectPolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Touches)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  EXPECT_TRUE(analyser.Touches(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.Touches(pointInsidePolygon));
  EXPECT_FALSE(analyser.Touches(pointOutsidePolygon));
  EXPECT_TRUE(analyser.Touches(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Touches(crossingLineGeometry));
  EXPECT_FALSE(analyser.Touches(disjointLineGeometry));
  EXPECT_FALSE(analyser.Touches(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Touches(noIntersectPolygonGeometry));
  EXPECT_TRUE(analyser.Touches(touchingPolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Contains)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  EXPECT_FALSE(analyser.Contains(pointOnPolygonEdgeGeometry));
  EXPECT_TRUE(analyser.Contains(pointInsidePolygon));
  EXPECT_FALSE(analyser.Contains(pointOutsidePolygon));
  EXPECT_FALSE(analyser.Contains(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Contains(crossingLineGeometry));
  EXPECT_FALSE(analyser.Contains(disjointLineGeometry));
  EXPECT_TRUE(analyser.Contains(insideLineGeometry));
  EXPECT_FALSE(analyser.Contains(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Contains(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Contains(touchingPolygonGeometry));
  EXPECT_TRUE(analyser.Contains(insidePolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Covers)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  EXPECT_TRUE(analyser.Covers(pointOnPolygonEdgeGeometry));
  EXPECT_TRUE(analyser.Covers(pointInsidePolygon));
  EXPECT_FALSE(analyser.Covers(pointOutsidePolygon));
  EXPECT_TRUE(analyser.Covers(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Covers(crossingLineGeometry));
  EXPECT_FALSE(analyser.Covers(disjointLineGeometry));
  EXPECT_TRUE(analyser.Covers(insideLineGeometry));
  EXPECT_FALSE(analyser.Covers(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Covers(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Covers(touchingPolygonGeometry));
  EXPECT_TRUE(analyser.Covers(insidePolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Within)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  Geometry outsidePolygonGeometry;
  outsidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingOutside;
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(outsidePolygonGeometry.m_polygon.outer(), outerRingOutside);

  EXPECT_FALSE(analyser.Within(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.Within(pointInsidePolygon));
  EXPECT_FALSE(analyser.Within(pointOutsidePolygon));
  EXPECT_FALSE(analyser.Within(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Within(crossingLineGeometry));
  EXPECT_FALSE(analyser.Within(disjointLineGeometry));
  EXPECT_FALSE(analyser.Within(insideLineGeometry));
  EXPECT_FALSE(analyser.Within(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Within(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Within(touchingPolygonGeometry));
  EXPECT_FALSE(analyser.Within(insidePolygonGeometry));
  EXPECT_TRUE(analyser.Within(outsidePolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, CoveredBy)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  Geometry outsidePolygonGeometry;
  outsidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingOutside;
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(outsidePolygonGeometry.m_polygon.outer(), outerRingOutside);

  EXPECT_FALSE(analyser.CoveredBy(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.CoveredBy(pointInsidePolygon));
  EXPECT_FALSE(analyser.CoveredBy(pointOutsidePolygon));
  EXPECT_FALSE(analyser.CoveredBy(boundaryLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(crossingLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(disjointLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(insideLineGeometry));
  EXPECT_FALSE(analyser.CoveredBy(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.CoveredBy(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.CoveredBy(touchingPolygonGeometry));
  EXPECT_FALSE(analyser.CoveredBy(insidePolygonGeometry));
  EXPECT_TRUE(analyser.CoveredBy(outsidePolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Crosses)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  Geometry outsidePolygonGeometry;
  outsidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingOutside;
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(outsidePolygonGeometry.m_polygon.outer(), outerRingOutside);

  EXPECT_FALSE(analyser.Crosses(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.Crosses(pointInsidePolygon));
  EXPECT_FALSE(analyser.Crosses(pointOutsidePolygon));
  EXPECT_FALSE(analyser.Crosses(boundaryLineGeometry));
  EXPECT_TRUE(analyser.Crosses(crossingLineGeometry));
  EXPECT_FALSE(analyser.Crosses(disjointLineGeometry));
  EXPECT_FALSE(analyser.Crosses(insideLineGeometry));
  EXPECT_FALSE(analyser.Crosses(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Crosses(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Crosses(touchingPolygonGeometry));
  EXPECT_FALSE(analyser.Crosses(insidePolygonGeometry));
  EXPECT_FALSE(analyser.Crosses(outsidePolygonGeometry));
}

UNIT_TEST(PolygonAnalyser, Overlaps)
{
  polygon_type polygon;

  linestring_type outerLine;
  outerLine.push_back(point_type(0.0, 0.0));
  outerLine.push_back(point_type(0.0, 5.0));
  outerLine.push_back(point_type(5.0, 5.0));
  outerLine.push_back(point_type(5.0, 0.0));
  outerLine.push_back(point_type(0.0, 0.0));
  boost::geometry::assign_points(polygon.outer(), outerLine);

  polygon.inners().resize(2);

  linestring_type innerLine1;
  innerLine1.push_back(point_type(1.0, 1.0));
  innerLine1.push_back(point_type(1.0, 2.0));
  innerLine1.push_back(point_type(2.0, 2.0));
  innerLine1.push_back(point_type(2.0, 1.0));
  innerLine1.push_back(point_type(1.0, 1.0));
  linestring_type innerLine2;
  innerLine2.push_back(point_type(3.0, 3.0));
  innerLine2.push_back(point_type(3.0, 4.0));
  innerLine2.push_back(point_type(4.0, 4.0));
  innerLine2.push_back(point_type(4.0, 3.0));
  innerLine2.push_back(point_type(3.0, 3.0));

  boost::geometry::assign_points(polygon.inners().at(0), innerLine1);
  boost::geometry::assign_points(polygon.inners().at(1), innerLine2);

  PolygonAnalyser analyser(polygon);

  // Define geometries to compare to the initial geometry
  Geometry pointOnPolygonEdgeGeometry;
  pointOnPolygonEdgeGeometry.m_type = GeometryType::POINT;
  pointOnPolygonEdgeGeometry.m_point = point_type(0.0, 3.0);

  Geometry pointInsidePolygon;
  pointInsidePolygon.m_type = GeometryType::POINT;
  pointInsidePolygon.m_point = point_type(2.5, 2.5);

  Geometry pointOutsidePolygon;
  pointOutsidePolygon.m_type = GeometryType::POINT;
  pointOutsidePolygon.m_point = point_type(10.0, 10.0);

  Geometry boundaryLineGeometry;
  boundaryLineGeometry.m_type = GeometryType::LINESTRING;
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 1.0));
  boundaryLineGeometry.m_linestring.push_back(point_type(0.0, 4.0));

  Geometry crossingLineGeometry;
  crossingLineGeometry.m_type = GeometryType::LINESTRING;
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, -1.0));
  crossingLineGeometry.m_linestring.push_back(point_type(0.5, 6.0));

  Geometry disjointLineGeometry;
  disjointLineGeometry.m_type = GeometryType::LINESTRING;
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, -1.0));
  disjointLineGeometry.m_linestring.push_back(point_type(-0.5, 6.0));

  Geometry insideLineGeometry;
  insideLineGeometry.m_type = GeometryType::LINESTRING;
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 1.0));
  insideLineGeometry.m_linestring.push_back(point_type(0.5, 3.0));

  Geometry partialIntersectPolygonGeometry;
  partialIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingPartialIntersect;
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, 1.0));
  outerRingPartialIntersect.push_back(point_type(1.0, -1.0));
  outerRingPartialIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(partialIntersectPolygonGeometry.m_polygon.outer(), outerRingPartialIntersect);

  Geometry noIntersectPolygonGeometry;
  noIntersectPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingNoIntersect;
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -2.0));
  outerRingNoIntersect.push_back(point_type(-2.0, -1.0));
  outerRingNoIntersect.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(noIntersectPolygonGeometry.m_polygon.outer(), outerRingNoIntersect);

  Geometry touchingPolygonGeometry;
  touchingPolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingTouching;
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, 2.0));
  outerRingTouching.push_back(point_type(0.0, -1.0));
  outerRingTouching.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(touchingPolygonGeometry.m_polygon.outer(), outerRingTouching);

  Geometry insidePolygonGeometry;
  insidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingInside;
  outerRingInside.push_back(point_type(0.5, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.5));
  outerRingInside.push_back(point_type(0.9, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.9));
  outerRingInside.push_back(point_type(0.5, 0.5));
  boost::geometry::assign_points(insidePolygonGeometry.m_polygon.outer(), outerRingInside);

  Geometry outsidePolygonGeometry;
  outsidePolygonGeometry.m_type = GeometryType::POLYGON;
  linestring_type outerRingOutside;
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, 10.0));
  outerRingOutside.push_back(point_type(10.0, -1.0));
  outerRingOutside.push_back(point_type(-1.0, -1.0));
  boost::geometry::assign_points(outsidePolygonGeometry.m_polygon.outer(), outerRingOutside);

  EXPECT_FALSE(analyser.Overlaps(pointOnPolygonEdgeGeometry));
  EXPECT_FALSE(analyser.Overlaps(pointInsidePolygon));
  EXPECT_FALSE(analyser.Overlaps(pointOutsidePolygon));
  EXPECT_FALSE(analyser.Overlaps(boundaryLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(crossingLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(disjointLineGeometry));
  EXPECT_FALSE(analyser.Overlaps(insideLineGeometry));
  EXPECT_TRUE(analyser.Overlaps(partialIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Overlaps(noIntersectPolygonGeometry));
  EXPECT_FALSE(analyser.Overlaps(touchingPolygonGeometry));
  EXPECT_FALSE(analyser.Overlaps(insidePolygonGeometry));
  EXPECT_FALSE(analyser.Overlaps(outsidePolygonGeometry));
}
