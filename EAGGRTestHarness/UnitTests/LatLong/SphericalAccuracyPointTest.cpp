//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file SphericalAccuracyPointTest.cpp
/// 
/// Tests for the EAGGR::LatLong::SphericalAccuracyPoint class.
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

#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR;
using namespace EAGGR::Utilities::Maths;

UNIT_TEST(SphericalAccuracyPoint, Getters)
{
  static const Degrees TOLERANCE = 1e-09;

  static const Degrees LATITUDE = 40.0;
  static const Degrees LONGITUDE = 50.0;
  static const Degrees ACCURACY = 1e-05;

  LatLong::SphericalAccuracyPoint point(LATITUDE, LONGITUDE, ACCURACY);

  EXPECT_DOUBLE_EQ(LATITUDE, point.GetLatitude());
  EXPECT_NEAR(0.698131701, point.GetLatitudeInRadians(), TOLERANCE);

  EXPECT_DOUBLE_EQ(LONGITUDE, point.GetLongitude());
  EXPECT_NEAR(0.872664626, point.GetLongitudeInRadians(), TOLERANCE);

  EXPECT_DOUBLE_EQ(ACCURACY, point.GetAccuracy());
}

UNIT_TEST(SphericalAccuracyPoint, InvalidConstruction)
{
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(90.0001, 0.0, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(-90.0001, 0.0, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(0.0, -180.0001, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(0.0, 180.0001, 1.0), EAGGRException);
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(0.0, 0.0, -0.0001), EAGGRException);
  EXPECT_THROW(LatLong::SphericalAccuracyPoint point(0.0, 0.0, 360.0001), EAGGRException);
}

UNIT_TEST(SphericalAccuracyPoint, SquareMetresToAngleAccuracy)
{
  // Test data generated based on equations in:
  // http://mathworld.wolfram.com/SphericalCap.html

  static const Degrees TOLERANCE = 1e-09;

  EXPECT_NEAR(0.000358777, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(5000.0), TOLERANCE);
  EXPECT_NEAR(0.000160450, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(1000.0), TOLERANCE);
  EXPECT_NEAR(0.000113456, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(500.0), TOLERANCE);
  EXPECT_NEAR(5.07403e-05, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(100.0), TOLERANCE);
  EXPECT_NEAR(3.58788e-05, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(50.0), TOLERANCE);
  EXPECT_NEAR(1.60410e-05, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(10.0), TOLERANCE);
  EXPECT_NEAR(1.13587e-05, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(5.0), TOLERANCE);
  EXPECT_NEAR(5.05099e-06, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(1.0), TOLERANCE);
  EXPECT_NEAR(3.62225e-06, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(0.5), TOLERANCE);
  EXPECT_NEAR(1.70755e-06, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(0.1), TOLERANCE);
  EXPECT_NEAR(5.07387e-08, LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(0.0001), TOLERANCE);

  EXPECT_THROW(LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(-0.0001), EAGGRException);
}

UNIT_TEST(SphericalAccuracyPoint, AngleAccuracyToSquareMetres)
{
  static const double TOLERANCE = 1e-09;

  // Test using values determined from SquareMetresToAngleAccuracy test
  // Some numerical rounding causes the values to be slightly different from the original accuracy values
  EXPECT_NEAR(4999.9991965996, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(0.000358776979069), TOLERANCE);
  EXPECT_NEAR(1000.0055021859, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(0.000160450397094), TOLERANCE);
  EXPECT_NEAR(500.0027510929, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(0.000113455563829), TOLERANCE);
  EXPECT_NEAR(100.0062130846, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(5.07403072385e-05), TOLERANCE);
  EXPECT_NEAR(50.0031065423, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(3.58788153279e-05), TOLERANCE);
  EXPECT_NEAR(9.9949584425, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.60409504699e-05), TOLERANCE);
  EXPECT_NEAR(5.0116363862, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.13587195893e-05), TOLERANCE);
  EXPECT_NEAR(0.9910015453, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(5.05099300793e-06), TOLERANCE);
  EXPECT_NEAR(0.5096579376, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(3.62225480918e-06), TOLERANCE);
  EXPECT_NEAR(0.1132573195, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(1.70754729250e-06), TOLERANCE);
  EXPECT_NEAR(0.0001, LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(5.07387e-08), TOLERANCE);

  EXPECT_THROW(LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(-0.0001), EAGGRException);
}

UNIT_TEST(SphericalAccuracyPoint, GetDistanceToPoint)
{
  // Test data generated using the forms in:
  // http://www.movable-type.co.uk/scripts/latlong.html

  static const double TOLERANCE = 0.1;

  LatLong::SphericalAccuracyPoint point1(10.0, -10.0, 1.0);
  LatLong::SphericalAccuracyPoint point2(-10.0, 10.0, 1.0);
  LatLong::SphericalAccuracyPoint point3(-10.01, 9.99, 1.0);

  EXPECT_DOUBLE_EQ(0.0, point1.GetDistanceToPoint(point1));
  EXPECT_DOUBLE_EQ(0.0, point2.GetDistanceToPoint(point2));
  EXPECT_DOUBLE_EQ(0.0, point3.GetDistanceToPoint(point3));

  EXPECT_NEAR(3137044.6, point1.GetDistanceToPoint(point2), TOLERANCE);
  EXPECT_NEAR(3137044.6, point2.GetDistanceToPoint(point1), TOLERANCE);

  EXPECT_NEAR(3137045.0, point1.GetDistanceToPoint(point3), TOLERANCE);
  EXPECT_NEAR(3137045.0, point3.GetDistanceToPoint(point1), TOLERANCE);

  EXPECT_NEAR(1560.6, point2.GetDistanceToPoint(point3), TOLERANCE);
  EXPECT_NEAR(1560.6, point3.GetDistanceToPoint(point2), TOLERANCE);
}
