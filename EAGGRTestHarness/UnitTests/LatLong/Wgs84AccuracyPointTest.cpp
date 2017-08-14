//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file Wgs84AccuracyPointTest.cpp
///
/// Tests for the EAGGR::LatLong::Wgs84AccuracyPoint class.
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

#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR;
using namespace EAGGR::Utilities::Maths;

UNIT_TEST(Wgs84AccuracyPoint , Getters)
{
  static const Degrees TOLERANCE = 1e-09;

  static const Degrees LATITUDE = 40.0;
  static const Degrees LONGITUDE = 50.0;
  static const Degrees ACCURACY = 1e-05;

  LatLong::Wgs84AccuracyPoint point(LATITUDE, LONGITUDE, ACCURACY);

  EXPECT_DOUBLE_EQ(LATITUDE, point.GetLatitude());
  EXPECT_NEAR(0.698131701, point.GetLatitudeInRadians(), TOLERANCE);

  EXPECT_DOUBLE_EQ(LONGITUDE, point.GetLongitude());
  EXPECT_NEAR(0.872664626, point.GetLongitudeInRadians(), TOLERANCE);

  EXPECT_DOUBLE_EQ(ACCURACY, point.GetAccuracy());
}

UNIT_TEST(Wgs84AccuracyPoint, Operators)
{
  static const Degrees LATITUDE = 40.0;
  static const Degrees LONGITUDE = 50.0;
  static const Degrees ACCURACY = 1e-05;

  LatLong::Wgs84AccuracyPoint point(LATITUDE, LONGITUDE, ACCURACY);
  LatLong::Wgs84AccuracyPoint samePoint(LATITUDE, LONGITUDE, ACCURACY);
  LatLong::Wgs84AccuracyPoint differentPoint(LATITUDE + 1.0, LONGITUDE, ACCURACY);

  // Test "equal to" operator
  EXPECT_TRUE(point == samePoint);
  EXPECT_FALSE(point == differentPoint);

  // Test "not equal to" operator
  EXPECT_FALSE(point != samePoint);
  EXPECT_TRUE(point != differentPoint);
}

UNIT_TEST(Wgs84AccuracyPoint, InvalidConstruction)
{
  EXPECT_THROW(LatLong::Wgs84AccuracyPoint point(90.0001, 0.0, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::Wgs84AccuracyPoint point(-90.0001, 0.0, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::Wgs84AccuracyPoint point(0.0, -180.0001, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::Wgs84AccuracyPoint point(0.0, 180.0001, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::Wgs84AccuracyPoint point(0.0, 0.0, -0.0001), EAGGRException);
}
