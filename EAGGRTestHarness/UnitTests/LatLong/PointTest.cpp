//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file PointTest.cpp
/// 
/// Tests for the EAGGR::Model::Point class.
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

#include "Src/LatLong/Point.hpp"
#include "Src/EAGGRException.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/Utilities/Maths.hpp"

using namespace EAGGR;
using namespace EAGGR::LatLong;
using namespace EAGGR::Utilities::Maths;

UNIT_TEST(Point, Getters)
{
  const double latitude = 1.234;
  const double longitude = 2.345;
  const Point point(latitude, longitude);

  EXPECT_DOUBLE_EQ(latitude, point.GetLatitude());
  EXPECT_DOUBLE_EQ(longitude, point.GetLongitude());
}

UNIT_TEST(Point, InvalidConstruction)
{
  EXPECT_THROW(Point point(90.0001, 0.0), EAGGRException);
  EXPECT_THROW(Point point(-90.0001, 0.0), EAGGRException);
  EXPECT_THROW(Point point(0.0, -180.0001), EAGGRException);
  EXPECT_THROW(Point point(0.0, 180.0001), EAGGRException);
}

UNIT_TEST(Point, WrapLongitude)
{
  EXPECT_EQ(0.0, LatLong::Point::WrapLongitude(0.0));
  EXPECT_EQ(-180.0, LatLong::Point::WrapLongitude(-180.0));
  EXPECT_EQ(180.0, LatLong::Point::WrapLongitude(180.0));
  EXPECT_EQ(0.0, LatLong::Point::WrapLongitude(360.0));
  EXPECT_EQ(0.0, LatLong::Point::WrapLongitude(-360.0));
  EXPECT_EQ(90.0, LatLong::Point::WrapLongitude(-270.0));
  EXPECT_EQ(-90.0, LatLong::Point::WrapLongitude(270.0));
  EXPECT_EQ(0.0, LatLong::Point::WrapLongitude(720.0));
  EXPECT_EQ(180.0, LatLong::Point::WrapLongitude(540.0));
  EXPECT_EQ(-180.0, LatLong::Point::WrapLongitude(-540.0));
}

UNIT_TEST(Point, IsCloseTo)
{
  static const Degrees TOLERANCE = 5.0;
  const LatLong::Point point(50.0, 179.0);

  EXPECT_TRUE(point.IsCloseTo(point, TOLERANCE, TOLERANCE));

  EXPECT_TRUE(point.IsCloseTo(LatLong::Point(48.0, 179.0), TOLERANCE, TOLERANCE));
  EXPECT_TRUE(point.IsCloseTo(LatLong::Point(50.0, 177.0), TOLERANCE, TOLERANCE));
  EXPECT_TRUE(point.IsCloseTo(LatLong::Point(50.0, -179.0), TOLERANCE, TOLERANCE));

  EXPECT_FALSE(point.IsCloseTo(LatLong::Point(40.0, 179.0), TOLERANCE, TOLERANCE));
  EXPECT_FALSE(point.IsCloseTo(LatLong::Point(50.0, 169.0), TOLERANCE, TOLERANCE));
  EXPECT_FALSE(point.IsCloseTo(LatLong::Point(50.0, -171.0), TOLERANCE, TOLERANCE));
}
