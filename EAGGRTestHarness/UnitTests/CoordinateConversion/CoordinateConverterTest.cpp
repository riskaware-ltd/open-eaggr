//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file CoordinateConverterTest.cpp
///
/// Tests for the EAGGR::CoordinateConversion::CoordinateConverter class.
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

#include <cstdlib>

#include "TestMacros.hpp"

#include "Src/CoordinateConversion/CoordinateConverter.hpp"
#include "Src/LatLong/Point.hpp"

using namespace EAGGR::CoordinateConversion;

static const double LAT_LONG_TOLERANCE = 1e-6;
static const double ACCURACY = 1.0;

UNIT_TEST(CoordinateConverter, ConvertWGS84ToSpherical)
{
  CoordinateConverter converter;

  // Expected values obtained by running proj4 on command line (cs2cs.exe)

  const double angleAccuracy = EAGGR::LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(ACCURACY);

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84Point(51.1879158, 90.0, ACCURACY);
  double expectedLatitude = 51.0;
  double expectedLongitude = 90.0;
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(wgs84Point);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  // Test points at poles
  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtNPole1(90.0, 0.0, ACCURACY);
  expectedLatitude = 90.0;
  expectedLongitude = 0.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtNPole1);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtNPole2(90.0, -100.0, ACCURACY);
  expectedLatitude = 90.0;
  expectedLongitude = 0.0;// longitude returned as zero on the poles to replicate previous proj4
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtNPole2);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtSPole1(-90.0, 0.0, ACCURACY);
  expectedLatitude = -90.0;
  expectedLongitude = 0.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtSPole1);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtSPole2(-90.0, -100.0, ACCURACY);
  expectedLatitude = -90.0;
  expectedLongitude = 0.0;//  longitude returned as zero on the poles to replicate previous proj4
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtSPole2);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  // Test points at +/-180 degrees longitude
  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtDateLine1(51.1879158, -180.0, ACCURACY);
  expectedLatitude = 51.0;
  expectedLongitude = -180.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtDateLine1);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointAtDateLine2(51.1879158, 180.0, ACCURACY);
  expectedLatitude = 51.0;
  expectedLongitude = 180.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointAtDateLine2);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  // Test points on equator
  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointOnEquator1(0.0, 0.0, ACCURACY);
  expectedLatitude = 0.0;
  expectedLongitude = 0.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointOnEquator1);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());

  EAGGR::LatLong::Wgs84AccuracyPoint wgs84PointOnEquator2(0.0, -100.0, ACCURACY);
  expectedLatitude = 0.0;
  expectedLongitude = -100.0;
  sphericalPoint = converter.ConvertWGS84ToSphere(wgs84PointOnEquator2);
  EXPECT_NEAR(expectedLatitude, sphericalPoint.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, sphericalPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(angleAccuracy, sphericalPoint.GetAccuracy());
}

UNIT_TEST(CoordinateConverter, ConvertSphericalToWGS84)
{
  CoordinateConverter converter;

  // Expected values obtained by running proj4 on command line (cs2cs.exe)

  const double squareMetreAccuracy = EAGGR::LatLong::SphericalAccuracyPoint::AngleAccuracyToSquareMetres(ACCURACY);

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint(51.0, 90.0, ACCURACY);
  double expectedLatitude = 51.1879158;
  double expectedLongitude = 90.0;
  EAGGR::LatLong::Wgs84AccuracyPoint wgs84Point = converter.ConvertSphereToWGS84(sphericalPoint);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  // Test points at poles
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtNPole1(90.0, 0.0, ACCURACY);
  expectedLatitude = 90.0;
  expectedLongitude = 0.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtNPole1);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtNPole2(90.0, -100.0, ACCURACY);
  expectedLatitude = 90.0;
  expectedLongitude = 0.0;// longitude returned as zero on the poles to replicate previous proj4
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtNPole2);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtSPole1(-90.0, 0.0, ACCURACY);
  expectedLatitude = -90.0;
  expectedLongitude = 0.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtSPole1);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtSPole2(-90.0, -100.0, ACCURACY);
  expectedLatitude = -90.0;
  expectedLongitude = 0.0;// longitude returned as zero on the poles to replicate previous proj4
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtSPole2);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  // Test points at +/-180 degrees longitude
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtDateLine1(51.0, -180.0, ACCURACY);
  expectedLatitude = 51.1879158;
  expectedLongitude = -180.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtDateLine1);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointAtDateLine2(51.0, 180.0, ACCURACY);
  expectedLatitude = 51.1879158;
  expectedLongitude = 180.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointAtDateLine2);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  // Test points on equator
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointOnEquator1(0.0, 0.0, ACCURACY);
  expectedLatitude = 0.0;
  expectedLongitude = 0.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointOnEquator1);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());

  EAGGR::LatLong::SphericalAccuracyPoint sphericalPointOnEquator2(0.0, -100.0, ACCURACY);
  expectedLatitude = 0.0;
  expectedLongitude = -100.0;
  wgs84Point = converter.ConvertSphereToWGS84(sphericalPointOnEquator2);
  EXPECT_NEAR(expectedLatitude, wgs84Point.GetLatitude(), LAT_LONG_TOLERANCE);
  EXPECT_NEAR(expectedLongitude, wgs84Point.GetLongitude(), LAT_LONG_TOLERANCE);
  EXPECT_EQ(squareMetreAccuracy, wgs84Point.GetAccuracy());
}

// This test was added to demonstrate a bug in the coordinate converter where converting a
// WGS84 point to spherical and back doesn't give the same point.
// Ticket #85 addressed this with a workaround, ticket #97 for further enhancements.
UNIT_TEST(CoordinateConverter, ConvertWGS84ToSphericalAndBackTest)
{
  CoordinateConverter converter;

  for (double latitude = 0.0; latitude <= 90.0; latitude += 0.1)
  {
    EAGGR::LatLong::Wgs84AccuracyPoint wgs84Point(latitude, 0.0, ACCURACY);

    EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(wgs84Point);
    EAGGR::LatLong::Wgs84AccuracyPoint convertedPoint = converter.ConvertSphereToWGS84(sphericalPoint);

    EXPECT_NEAR(wgs84Point.GetLatitude(), convertedPoint.GetLatitude(), LAT_LONG_TOLERANCE);
    EXPECT_NEAR(wgs84Point.GetLongitude(), convertedPoint.GetLongitude(), LAT_LONG_TOLERANCE);
  }
}
