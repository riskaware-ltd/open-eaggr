//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file SnyderTest.cpp
///
/// Tests the Snyder class.
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
#include "Src/Model/FaceTypes.hpp"
#include "Src/Model/FaceCoordinate.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IProjection/Snyder.hpp"

static const double FACE_OFFSET_TOLERANCE = 1e-6;
static const double LAT_LONG_TOLERANCE = 1e-6;

static const double AREA_ACCURACY_TOLERANCE = 1e-11;
static const double ANGLE_ACCURACY_TOLERANCE = 1e-6;

using namespace EAGGR;

/// Tests that converting the face centres result in a coordinate of 0, 0 on the correct face
UNIT_TEST(Snyder_Icosahedron, FaceCentres)
{
  static const Model::FaceIndex NO_OF_FACES = 20U;
  double pointData[NO_OF_FACES][3] =
  {
    // Lat, Long, Expected Face
    { 52.62263186, -144.0, 0U},
    { 52.62263186, -72.0, 1U},
    { 52.62263186, 0.0, 2U},
    { 52.62263186, 72.0, 3U},
    { 52.62263186, 144.0, 4U},
    { 10.81231696, -144.0, 5U},
    { 10.81231696, -72.0, 6U},
    { 10.81231696, 0.0, 7U},
    { 10.81231696, 72.0, 8U},
    { 10.81231696, 144.0, 9U},
    { -10.81231696, -108.0, 10U},
    { -10.81231696, -36.0, 11U},
    { -10.81231696, 36.0, 12U},
    { -10.81231696, 108.0, 13U},
    { -10.81231696, 180.0, 14U},
    { -52.62263186, -108.0, 15U},
    { -52.62263186, -36.0, 16U},
    { -52.62263186, 36.0, 17U},
    { -52.62263186, 108.0, 18U},
    { -52.62263186, 180.0, 19U}};

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (Model::FaceIndex face = 0U; face < NO_OF_FACES; face++)
  {
    // Lat / long coordinates for the face centres
    const LatLong::SphericalAccuracyPoint originalPoint(pointData[face][0], pointData[face][1], 0.1);

    // Convert the point
    const Model::FaceCoordinate faceCoord = projection.GetFaceCoordinate(originalPoint);

    // Check the coordinates we get back
    EXPECT_EQ(pointData[face][2], faceCoord.GetFaceIndex());
    EXPECT_NEAR(1.523087e-5, faceCoord.GetAccuracy(), AREA_ACCURACY_TOLERANCE);
    EXPECT_DOUBLE_EQ(0.0, faceCoord.GetXOffset());
    EXPECT_DOUBLE_EQ(0.0, faceCoord.GetYOffset());

    // Convert the cell back to a lat / long to test the inverse transform
    const LatLong::SphericalAccuracyPoint returnedPoint = projection.GetLatLongPoint(faceCoord);
    EXPECT_NEAR(originalPoint.GetLatitude(), returnedPoint.GetLatitude(), LAT_LONG_TOLERANCE);
    EXPECT_NEAR(originalPoint.GetLongitude(), returnedPoint.GetLongitude(), LAT_LONG_TOLERANCE);
    EXPECT_NEAR(originalPoint.GetAccuracy(), returnedPoint.GetAccuracy(), ANGLE_ACCURACY_TOLERANCE);
  }
}

/// Converts points at the 3 vertices on each face and ensures the faces
/// and coordinates are as expected.  Also ensures the values convert back to the correct lat/long point.
UNIT_TEST(Snyder_Icosahedron, Vertices)
{
  static const unsigned short NO_OF_POINTS = 60U;
  double pointData[NO_OF_POINTS][5] =
  {
    // Lat, Long, Expected Face, Expected X, Expected Y
    { 89.99999, -144.0, 0U, 0, 0.577350},
    { 26.56506, -108.000001, 0U, 0.5, -0.288675},
    { 26.56506, -179.999999, 0U, -0.5, -0.288675},
    { 89.99999, -72.0, 1U, 0, 0.577350},
    { 26.56506, -36.000001, 1U, 0.5, -0.288675},
    { 26.56506, -107.999999, 1U, -0.5, -0.288675},
    { 89.99999, 0.0, 2U, 0, 0.577350},
    { 26.56506, 35.999999, 2U, 0.5, -0.288675},
    { 26.56506, -35.999999, 2U, -0.5, -0.288675},
    { 89.99999, 72.0, 3U, 0, 0.577350},
    { 26.56506, 107.999999, 3U, 0.5, -0.288675},
    { 26.56506, 36.000001, 3U, -0.5, -0.288675},
    { 89.99999, 144.0, 4U, 0, 0.577350},
    { 26.56506, 179.999999, 4U, 0.5, -0.288675},
    { 26.56506, 108.000001, 4U, -0.5, -0.288675},
    { 26.56505, -179.999999, 5U, 0.0, 0.577350},
    { 26.56505, -108.000001, 5U, 0.5, -0.288675},
    { -26.56505, -144.0, 5U, -0.5, -0.288675},
    { 26.56505, -107.999999, 6U, 0.0, 0.577350},
    { 26.56505, -36.000001, 6U, 0.5, -0.288675},
    { -26.56505, -72.0, 6U, -0.5, -0.288675},
    { 26.56505, -35.999999, 7U, 0.0, 0.577350},
    { 26.56505, 35.999999, 7U, 0.5, -0.288675},
    { -26.56505, 0.0, 7U, -0.5, -0.288675},
    { 26.56505, 36.000001, 8U, 0.0, 0.577350},
    { 26.56505, 107.999999, 8U, 0.5, -0.288675},
    { -26.56505, 72.0, 8U, -0.5, -0.288675},
    { 26.56505, 108.000001, 9U, 0.0, 0.577350},
    { 26.56505, 179.999999, 9U, 0.5, -0.288675},
    { -26.56505, 144.0, 9U, -0.5, -0.288675},
    { 26.56505, -108.0, 10U, 0.0, 0.577350},
    { -26.56505, -72.000001, 10U, 0.5, -0.288675},
    { -26.56505, -143.999999, 10U, -0.5, -0.288675},
    { 26.56505, -36.0, 11U, 0.0, 0.577350},
    { -26.56505, -0.000001, 11U, 0.5, -0.288675},
    { -26.56505, -71.999999, 11U, -0.5, -0.288675},
    { 26.56505, 36.0, 12U, 0.0, 0.577350},
    { -26.56505, 71.999999, 12U, 0.5, -0.288675},
    { -26.56505, 0.000001, 12U, -0.5, -0.288675},
    { 26.56505, 108.0, 13U, 0.0, 0.577350},
    { -26.56505, 143.999999, 13U, 0.5, -0.288675},
    { -26.56505, 72.000001, 13U, -0.5, -0.288675},
    { 26.56505, 180.0, 14U, 0.0, 0.577350},
    { -26.56505, -144.000001, 14U, 0.5, -0.288675},
    { -26.56505, 144.000001, 14U, -0.5, -0.288675},
    { -26.56506, -143.999999, 15U, 0.0, 0.577350},
    { -26.56506, -72.000001, 15U, 0.5, -0.288675},
    { -89.99999, -108.0, 15U, -0.5, -0.288675},
    { -26.56506, -71.999999, 16U, 0.0, 0.577350},
    { -26.56506, -0.000001, 16U, 0.5, -0.288675},
    { -89.99999, -36.0, 16U, -0.5, -0.288675},
    { -26.56506, 0.000001, 17U, 0.0, 0.577350},
    { -26.56506, 71.999999, 17U, 0.5, -0.288675},
    { -89.99999, 36.0, 17U, -0.5, -0.288675},
    { -26.56506, 72.000001, 18U, 0.0, 0.577350},
    { -26.56506, 143.999999, 18U, 0.5, -0.288675},
    { -89.99999, 108.0, 18U, -0.5, -0.288675},
    { -26.56506, 144.000001, 19U, 0.0, 0.577350},
    { -26.56506, -144.000001, 19U, 0.5, -0.288675},
    { -89.99999, 180.0, 19U, -0.5, -0.288675}};

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short point = 0U; point < NO_OF_POINTS; point++)
  {
    // Lat / long coordinates for the vertices
    const LatLong::SphericalAccuracyPoint originalPoint(pointData[point][0], pointData[point][1], 0.1);

    // Convert the point
    const Model::FaceCoordinate faceCoord = projection.GetFaceCoordinate(originalPoint);

    // Check the coordinates we get back
    EXPECT_EQ(pointData[point][2], faceCoord.GetFaceIndex());
    EXPECT_NEAR(1.523087e-5, faceCoord.GetAccuracy(), AREA_ACCURACY_TOLERANCE);
    EXPECT_NEAR(pointData[point][3], faceCoord.GetXOffset(), FACE_OFFSET_TOLERANCE);
    EXPECT_NEAR(pointData[point][4], faceCoord.GetYOffset(), FACE_OFFSET_TOLERANCE);

    // Convert the cell back to a lat / long point to test the inverse transform
    const LatLong::SphericalAccuracyPoint returnedPoint = projection.GetLatLongPoint(faceCoord);
    EXPECT_TRUE(originalPoint.IsCloseTo(returnedPoint, LAT_LONG_TOLERANCE, LAT_LONG_TOLERANCE));
    EXPECT_NEAR(originalPoint.GetAccuracy(), returnedPoint.GetAccuracy(), ANGLE_ACCURACY_TOLERANCE);
  }
}

/// Ensures that a point on the exact edge between two faces is converted correctly
UNIT_TEST(Snyder_Icosahedron, PointOnFaceEdge)
{
  static const Model::FaceIndex NO_OF_POINTS = 2U;

  // Only edges with known longitudes are between faces 0, 4 and 16, 17.
  // The projection should choose the face with the smaller index
  double pointData[NO_OF_POINTS][3] =
  {
    // Lat, Long, Expected Face
    { 75.0, -180.0, 0U},
    { -75.0, 0.0, 16U}
  };

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short point = 0U; point < NO_OF_POINTS; point++)
  {
    // Lat / long coordinates for the point
    const LatLong::SphericalAccuracyPoint originalPoint(pointData[point][0], pointData[point][1], 0.1);

    // Convert the point
    const Model::FaceCoordinate faceCoord = projection.GetFaceCoordinate(originalPoint);

    // Check the coordinates we get back
    EXPECT_EQ(pointData[point][2], faceCoord.GetFaceIndex());

    // Convert the cell back to a lat / long point to test the inverse transform
    const LatLong::SphericalAccuracyPoint returnedPoint = projection.GetLatLongPoint(faceCoord);
    EXPECT_NEAR(originalPoint.GetLatitude(), returnedPoint.GetLatitude(), LAT_LONG_TOLERANCE);

    // Converted point can be returned at +/-180 degrees if original is at -/+180 degrees
    double longitudeDelta = originalPoint.GetLongitude() - returnedPoint.GetLongitude();
    if (longitudeDelta < -180.0)
    {
      longitudeDelta += 360.0;
    }

    if (longitudeDelta > 180.0)
    {
      longitudeDelta -= 360.0;
    }

    EXPECT_NEAR(0.0, longitudeDelta, LAT_LONG_TOLERANCE);
    EXPECT_NEAR(originalPoint.GetAccuracy(), returnedPoint.GetAccuracy(), ANGLE_ACCURACY_TOLERANCE);
  }
}

/// Ensures that a point on the exact edge between two faces is converted correctly
UNIT_TEST(Snyder_Icosahedron, PointOnVertex)
{
  static const Model::FaceIndex NO_OF_POINTS = 8U;

  // Only vertices with exact coordinates are over the poles..
  // The projection should choose the face with the smallest index
  double pointData[NO_OF_POINTS][3] =
  {
    // Lat, Long, Expected Face
    { 90.0, 0.0, 0U},
    { 90.0, -90.0, 0U},
    { 90.0, 90.0, 0U},
    { 90.0, 180.0, 0U},
    { -90.0, 0.0, 15U},
    { -90.0, -90.0, 15U},
    { -90.0, 90.0, 15U},
    { -90.0, 180.0, 15U}
  };

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short point = 0U; point < NO_OF_POINTS; point++)
  {
    // Lat / long coordinates for the vertices
    const LatLong::SphericalAccuracyPoint originalPoint(pointData[point][0], pointData[point][1], 0.1);

    // Convert the point
    const Model::FaceCoordinate faceCoord = projection.GetFaceCoordinate(originalPoint);

    // Check the coordinates we get back
    EXPECT_EQ(pointData[point][2], faceCoord.GetFaceIndex());
  }
}

/// Takes a city in each of the Northern/Eastern, Northern/Western, Southern/Eastern
/// and Southern/Western sphere quadrants.  Expected values have been shown to be
/// reasonable (expected face in approximately the correct location) and are
/// used here to form a regression test.
UNIT_TEST(Snyder_Icosahedron, GetFaceCoordinate_Cities)
{
  static const unsigned short NO_OF_CITIES = 4U;
  double pointData[NO_OF_CITIES][5] =
  {
    // Lat, Long, Expected Face, Expected X, Expected Y
    { 51.5072, -0.1275, 2U, -0.001199927, -0.015491116}, // London
    { -33.8650, 151.2094, 19U, 0.006219889, 0.431117808}, // Sydney
    { 1.3000, 103.8000, 13U, -0.056830965, 0.180075315}, // Singapore
    { -22.9068, -43.1729, 11U, -0.100838626, -0.171417009}  // Rio de Janeiro
  };

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short city = 0U; city < NO_OF_CITIES; city++)
  {
    // Lat / long coordinates for the city
    const LatLong::SphericalAccuracyPoint point(pointData[city][0], pointData[city][1], 0.1);

    // Convert the point
    const Model::FaceCoordinate output = projection.GetFaceCoordinate(point);

    // Check the coordinates we get back
    EXPECT_EQ(pointData[city][2], output.GetFaceIndex());
    EXPECT_NEAR(1.523087e-5, output.GetAccuracy(), AREA_ACCURACY_TOLERANCE);
    EXPECT_NEAR(pointData[city][3], output.GetXOffset(), FACE_OFFSET_TOLERANCE);
    EXPECT_NEAR(pointData[city][4], output.GetYOffset(), FACE_OFFSET_TOLERANCE);
  }
}

// Takes a city in each of the Northern/Eastern, Northern/Western, Southern/Eastern
// and Southern/Western sphere quadrants.  X-Y values are those found from the
// GetFaceCoordinate_Cities test and shows that they are converted back to
// the correct lat/long coordinates
UNIT_TEST(Snyder_Icosahedron, GetLatLongPoint_Cities)
{
  static const unsigned short NO_OF_CITIES = 4U;
  double coordData[NO_OF_CITIES][5] =
  {
    // Face, X, Y, Expected Lat, Expected Long
    { 2U, -0.001199927, -0.015491116, 51.5072, -0.1275}, // London
    { 19U, 0.006219889, 0.431117807, -33.8650, 151.2094}, // Sydney
    { 13U, -0.056830965, 0.180075315, 1.3000, 103.8000}, // Singapore
    { 11U, -0.100838626, -0.171417008, -22.9068, -43.1729}  // Rio de Janeiro
  };

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short city = 0U; city < NO_OF_CITIES; city++)
  {
    // Face coordinate for the city
    const Model::FaceCoordinate coord(
        coordData[city][0],
        coordData[city][1],
        coordData[city][2],
        1.523087e-5);

    // Convert the coordinate
    const LatLong::SphericalAccuracyPoint output = projection.GetLatLongPoint(coord);

    // Check the lat / long point we get back
    EXPECT_NEAR(0.1, output.GetAccuracy(), ANGLE_ACCURACY_TOLERANCE);
    EXPECT_NEAR(coordData[city][3], output.GetLatitude(), LAT_LONG_TOLERANCE);
    EXPECT_NEAR(coordData[city][4], output.GetLongitude(), LAT_LONG_TOLERANCE);
  }
}

UNIT_TEST(Snyder_Icosahedron, Accuracy)
{
  static const unsigned short NO_OF_ACCURACIES = 8U;
  double coordData[NO_OF_ACCURACIES][3] =
  {
    // Angle Accuracy, Expected Accuracy Area, Area Tolerance
    { 100.0, 1.000000, 1e-6},       // Limited to face area
    { 10.0, 1.519225e-1, 1e-7},     // 9700 km2
    { 1.0, 1.523048e-3, 1e-9},      // 97 km2
    { 0.1, 1.523087e-5, 1e-11},     // 970000 m2
    { 0.01, 1.523087e-7, 1e-13},    // 9700 m2
    { 0.001, 1.523087e-9, 1e-15},   // 97 m2
    { 0.0001, 1.523115e-11, 1e-17},  // 0.97 m2
    { 1E-8, 1.523087e-19, 1e-26}  // 0.97 m2
  };

  // Setup the model
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder projection(&globe);

  // Loop through the points
  for (unsigned short accuracy = 0U; accuracy < NO_OF_ACCURACIES; accuracy++)
  {
    // Location does not affect the calculation (this assumption is tested in the other tests)
    const LatLong::SphericalAccuracyPoint originalPoint(0.0, 0.0, coordData[accuracy][0]);

    // Convert the coordinate and check the accuracy area
    const Model::FaceCoordinate faceCoord = projection.GetFaceCoordinate(originalPoint);
    EXPECT_NEAR(coordData[accuracy][1], faceCoord.GetAccuracy(), coordData[accuracy][2]);

    // Convert the coordinate back again and check we get the original accuracy angle
    const LatLong::SphericalAccuracyPoint returnedPoint = projection.GetLatLongPoint(faceCoord);
    if (accuracy == 0U)
    {
      // Area has been limited so the returned point is not the same in this case
      EXPECT_NEAR(25.841933, returnedPoint.GetAccuracy(), ANGLE_ACCURACY_TOLERANCE);
    }
    else
    {
      EXPECT_NEAR(
          originalPoint.GetAccuracy(),
          returnedPoint.GetAccuracy(),
          ANGLE_ACCURACY_TOLERANCE);
    }
  }
}
