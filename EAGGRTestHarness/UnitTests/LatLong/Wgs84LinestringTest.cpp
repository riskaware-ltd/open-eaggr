//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file Wgs84LinestringTest.cpp
/// 
/// Tests for the EAGGR::LatLong::Wgs84Linestring class.
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
#include "Src/LatLong/Wgs84Linestring.hpp"

using namespace EAGGR::LatLong;

UNIT_TEST(Wgs84Linestring, BasicUsage)
{
  Wgs84Linestring linestring;

  // Check that trying to get a point from an empty linestring causes an error
  EXPECT_THROW(linestring.GetAccuracyPoint(0U), EAGGR::EAGGRException);

  const Wgs84AccuracyPoint expectedPoint1(1.0, 2.0, 3.0);
  const Wgs84AccuracyPoint expectedPoint2(4.0, 5.0, 6.0);
  const Wgs84AccuracyPoint expectedPoint3(7.0, 8.0, 9.0);

  linestring.AddAccuracyPoint(expectedPoint1.GetLatitude(), expectedPoint1.GetLongitude(), expectedPoint1.GetAccuracy());
  linestring.AddAccuracyPoint(expectedPoint2.GetLatitude(), expectedPoint2.GetLongitude(), expectedPoint2.GetAccuracy());
  linestring.AddAccuracyPoint(expectedPoint3.GetLatitude(), expectedPoint3.GetLongitude(), expectedPoint3.GetAccuracy());

  EXPECT_EQ(3U, linestring.GetNumberOfPoints());

  unsigned short pointIndex = 0U;
  EXPECT_TRUE(expectedPoint1 == *(linestring.GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(expectedPoint2 == *(linestring.GetAccuracyPoint(pointIndex)));
  pointIndex++;
  EXPECT_TRUE(expectedPoint3 == *(linestring.GetAccuracyPoint(pointIndex)));

  // Check that trying to get a point beyond the end of the linestring causes an error
  pointIndex++;
  EXPECT_THROW(linestring.GetAccuracyPoint(pointIndex), EAGGR::EAGGRException);
}

UNIT_TEST(Wgs84Linestring, Operators)
{
  Wgs84Linestring linestring;
  linestring.AddAccuracyPoint(1.0, 2.0, 3.0);
  linestring.AddAccuracyPoint(4.0, 5.0, 6.0);

  Wgs84Linestring sameLinestring;
  sameLinestring.AddAccuracyPoint(1.0, 2.0, 3.0);
  sameLinestring.AddAccuracyPoint(4.0, 5.0, 6.0);

  Wgs84Linestring differentLinestring;
  differentLinestring.AddAccuracyPoint(1.0, 2.0, 3.0);
  differentLinestring.AddAccuracyPoint(4.0, 5.0, 7.0);

  // Test "equal to" operator
  EXPECT_TRUE(linestring == sameLinestring);
  EXPECT_FALSE(linestring == differentLinestring);

  // Test "not equal to" operator
  EXPECT_FALSE(linestring != sameLinestring);
  EXPECT_TRUE(linestring != differentLinestring);
}
