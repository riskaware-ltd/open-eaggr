//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file Wgs84PolygonTest.cpp
/// 
/// Tests for the EAGGR::LatLong::Wgs84Polygon class.
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
#include "Src/LatLong/Wgs84Polygon.hpp"

using namespace EAGGR::LatLong;

UNIT_TEST(Wgs84Polygon, BasicUsage)
{
  Wgs84Polygon polygon;

  // Check that trying to get an inner ring from an empty polygon causes an error
  EXPECT_THROW(polygon.GetInnerRing(0U), EAGGR::EAGGRException);

  const Wgs84AccuracyPoint outerRingPoint1(10.0, 35.0, 0.01);
  const Wgs84AccuracyPoint outerRingPoint2(45.0, 45.0, 0.02);
  const Wgs84AccuracyPoint outerRingPoint3(40.0, 15.0, 0.03);
  const Wgs84AccuracyPoint outerRingPoint4(20.0, 10.0, 0.04);

  polygon.AddAccuracyPointToOuterRing(outerRingPoint1.GetLatitude(), outerRingPoint1.GetLongitude(), outerRingPoint1.GetAccuracy());
  polygon.AddAccuracyPointToOuterRing(outerRingPoint2.GetLatitude(), outerRingPoint2.GetLongitude(), outerRingPoint2.GetAccuracy());
  polygon.AddAccuracyPointToOuterRing(outerRingPoint3.GetLatitude(), outerRingPoint3.GetLongitude(), outerRingPoint3.GetAccuracy());
  polygon.AddAccuracyPointToOuterRing(outerRingPoint4.GetLatitude(), outerRingPoint4.GetLongitude(), outerRingPoint4.GetAccuracy());
  polygon.AddAccuracyPointToOuterRing(outerRingPoint1.GetLatitude(), outerRingPoint1.GetLongitude(), outerRingPoint1.GetAccuracy());

  const Wgs84AccuracyPoint innerRing1Point1(30.0, 20.0, 0.05);
  const Wgs84AccuracyPoint innerRing1Point2(35.0, 35.0, 0.06);
  const Wgs84AccuracyPoint innerRing1Point3(20.0, 30.0, 0.07);

  polygon.CreateInnerRing();
  polygon.AddAccuracyPointToInnerRing(0U, innerRing1Point1.GetLatitude(), innerRing1Point1.GetLongitude(), innerRing1Point1.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(0U, innerRing1Point2.GetLatitude(), innerRing1Point2.GetLongitude(), innerRing1Point2.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(0U, innerRing1Point3.GetLatitude(), innerRing1Point3.GetLongitude(), innerRing1Point3.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(0U, innerRing1Point1.GetLatitude(), innerRing1Point1.GetLongitude(), innerRing1Point1.GetAccuracy());

  const Wgs84AccuracyPoint innerRing2Point1(20.0, 10.0, 0.08);
  const Wgs84AccuracyPoint innerRing2Point2(30.0, 15.0, 0.09);
  const Wgs84AccuracyPoint innerRing2Point3(35.0, 15.0, 0.10);
  const Wgs84AccuracyPoint innerRing2Point4(30.0, 10.0, 0.11);

  polygon.CreateInnerRing();
  polygon.AddAccuracyPointToInnerRing(1U, innerRing2Point1.GetLatitude(), innerRing2Point1.GetLongitude(), innerRing2Point1.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(1U, innerRing2Point2.GetLatitude(), innerRing2Point2.GetLongitude(), innerRing2Point2.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(1U, innerRing2Point3.GetLatitude(), innerRing2Point3.GetLongitude(), innerRing2Point3.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(1U, innerRing2Point4.GetLatitude(), innerRing2Point4.GetLongitude(), innerRing2Point4.GetAccuracy());
  polygon.AddAccuracyPointToInnerRing(1U, innerRing2Point1.GetLatitude(), innerRing2Point1.GetLongitude(), innerRing2Point1.GetAccuracy());

  // Check the outer ring of the polygon

  const Wgs84Linestring * pOuterRing = polygon.GetOuterRing();
  EXPECT_EQ(5U, pOuterRing->GetNumberOfPoints());

  unsigned short pointIndex = 0U;
  EXPECT_TRUE(outerRingPoint1 == *(pOuterRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(outerRingPoint2 == *(pOuterRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(outerRingPoint3 == *(pOuterRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(outerRingPoint4 == *(pOuterRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(outerRingPoint1 == *(pOuterRing->GetAccuracyPoint(pointIndex)));

  pointIndex++;
  EXPECT_THROW(pOuterRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check the number of inner rings that the polygon has

  ASSERT_EQ(2U, polygon.GetNumberOfInnerRings());

  // Check the first inner ring of the polygon

  unsigned short ringIndex = 0U;
  const Wgs84Linestring * pInnerRing = polygon.GetInnerRing(ringIndex);
  EXPECT_EQ(4U, pInnerRing->GetNumberOfPoints());

  pointIndex = 0U;
  EXPECT_TRUE(innerRing1Point1 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing1Point2 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing1Point3 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing1Point1 == *(pInnerRing->GetAccuracyPoint(pointIndex)));

  pointIndex++;
  EXPECT_THROW(pInnerRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check the second inner ring of the polygon

  ringIndex++;
  pInnerRing = polygon.GetInnerRing(ringIndex);
  EXPECT_EQ(5U, pInnerRing->GetNumberOfPoints());

  pointIndex = 0U;
  EXPECT_TRUE(innerRing2Point1 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing2Point2 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing2Point3 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing2Point4 == *(pInnerRing->GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(innerRing2Point1 == *(pInnerRing->GetAccuracyPoint(pointIndex)));

  pointIndex++;
  EXPECT_THROW(pInnerRing->GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);

  // Check that trying to get a non-existent inner ring causes an error
  ringIndex++;
  EXPECT_THROW(polygon.GetInnerRing(ringIndex), EAGGR::EAGGRException);
}

UNIT_TEST(Wgs84Polygon, Operators)
{
  Wgs84Polygon polygon;
  polygon.AddAccuracyPointToOuterRing(1.0, 2.0, 3.0);

  Wgs84Polygon samePolygon;
  samePolygon.AddAccuracyPointToOuterRing(1.0, 2.0, 3.0);

  Wgs84Polygon differentPoint;
  differentPoint.AddAccuracyPointToOuterRing(1.0, 2.0, 4.0);

  Wgs84Polygon differentNoOfInnerRings;
  differentNoOfInnerRings.AddAccuracyPointToOuterRing(1.0, 2.0, 3.0);
  differentNoOfInnerRings.CreateInnerRing();

  // Test "equal to" operator
  EXPECT_TRUE(polygon == samePolygon);
  EXPECT_FALSE(polygon == differentPoint);
  EXPECT_FALSE(polygon == differentNoOfInnerRings);

  // Test "not equal to" operator
  EXPECT_FALSE(polygon != samePolygon);
  EXPECT_TRUE(polygon != differentPoint);
  EXPECT_TRUE(polygon != differentNoOfInnerRings);
}
