//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Evaluation Framework
//
//------------------------------------------------------
/// @file Performance.cpp
/// 
/// Implements the EvaluationFramework::Performance class.
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

#include <memory>
#include <cmath>

#include "gtest/gtest.h"
#include "Performance.hpp"

#include "Src/Utilities/RadianMacros.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/CoordinateConversion/CoordinateConverter.hpp"

using namespace EAGGR;
using namespace EAGGR::Utilities::Maths;

namespace EvaluationFramework
{
  Performance::Performance(const Model::DGGS & a_dggs)
      : m_dggs(a_dggs)
  {
  }

  void Performance::ConvertPointsWorldwide() const
  {
    // Minimum spherical distance (in metres) between the rings of points at constant latitudes
    static const double LAITUDE_SPACING = 100e3;

    // Average spherical distance (in metres) between the points on those rings
    static const double LONGITUDE_SPACING = 100e3;

    // Specifies a margin around the poles where points will be omitted from the test.
    // This feature has been included to avoid latitude values of -90 and 90 degrees
    // where the same point has multiple longitude values.
    static const Degrees LAT_LIMITS_MARGIN = 0.001;

    const Degrees latitudeStep = CalculateLatitudeStep(LAITUDE_SPACING, LAT_LIMITS_MARGIN);

    // Accuracy to be tested (in metres squared, equivalent angle, radius shown in comments)
    static const double ACCURACIES[] =
    { 3.141e6, // 0.00899236, 1000
        1.767e6, // 0.00674463, 750
        7.854e5, // 0.00449661, 500
        1.963e5, // 0.00224802, 250
        3.141e4, // 0.000899236, 100
        7.853e3, // 0.000449661, 50
        3.141e2, // 0.0000899236, 10
        7.853e1, // 0.0000449661, 5
        3.141e0, // 0.00000899236, 1
        7.853e-1 // 0.00000449661, 0.5
        };

    EAGGR::CoordinateConversion::CoordinateConverter coordinateConverter;

    // Create timers to record the performance
    Timers timers;

    unsigned long noOfConversions = 0UL;
    unsigned long noOfOneFacePoints = 0L;
    unsigned long noOfTwoFacePoints = 0L;
    unsigned long noOfManyFacePoints = 0L;

    for (unsigned short accuracyLevel = 0U;
        accuracyLevel < sizeof(ACCURACIES) / sizeof(*ACCURACIES); accuracyLevel++)
    {
      // Initialise running averages
      RunningAverage conversionErrorAverage;
      RunningAverage accuracyErrorAverage;

      // Get the accuracy area
      const double accuracyArea = ACCURACIES[accuracyLevel];

      // Calculate tolerances (based on the accuracy area)
      double conversionTolerance;
      Degrees accuracyTolerance;
      CalculateTolerances(accuracyArea, conversionTolerance, accuracyTolerance);

      for (Degrees latitude = LatLong::Point::m_MIN_LATITUDE + LAT_LIMITS_MARGIN;
          latitude <= LatLong::Point::m_MAX_LATITUDE - LAT_LIMITS_MARGIN; latitude += latitudeStep)
      {
        // Number of longitude steps based on the circumference around the current latitude
        const double circumference = CircumferenceAtLatitude(latitude);
        double noOfLongitudeSteps = round(circumference / LONGITUDE_SPACING);

        // Make sure at least one conversion is done at the current latitude
        noOfLongitudeSteps = (noOfLongitudeSteps > 0.0) ? noOfLongitudeSteps : 1.0;

        // Determine the size of the longitude step based on the number of steps required
        const Degrees longitudeStep = (LatLong::Point::m_MAX_LONGITUDE
            - LatLong::Point::m_MIN_LONGITUDE) / noOfLongitudeSteps;

        for (Degrees longitude = LatLong::Point::m_MIN_LONGITUDE;
            longitude <= LatLong::Point::m_MAX_LONGITUDE; longitude += longitudeStep)
        {
          const LatLong::Wgs84AccuracyPoint originalPoint(latitude, longitude, accuracyArea);

          timers.m_convertWgs84ToSpherical.Start();
          const LatLong::SphericalAccuracyPoint originalSphericalPoint =
              coordinateConverter.ConvertWGS84ToSphere(originalPoint);
          timers.m_convertWgs84ToSpherical.Pause();

          // Convert lat / long point to a cell
          timers.m_convertLatLongPoints.Start();
          std::unique_ptr < Model::Cell::ICell > pCell = m_dggs.ConvertLatLongPointToCell(
              originalSphericalPoint);
          timers.m_convertLatLongPoints.Pause();

          // Get cell location from cell
          Model::Cell::CellLocation location = pCell.get()->GetCellLocation();

          switch (location)
          {
            case Model::Cell::CellLocation::FACE:
              ++noOfOneFacePoints;
              break;
            case Model::Cell::CellLocation::EDGE:
              ++noOfTwoFacePoints;
              break;
            case Model::Cell::CellLocation::VERTEX:
              ++noOfManyFacePoints;
              break;
            case Model::Cell::CellLocation::UNKNOWN:
              // No need to do anything - number in this category can be determined from
              // the total number of points minus the other categories.
              break;
          }

          // Generate cell id from cell
          timers.m_generateCellIds.Start();
          const Model::Cell::DggsCellId cellId = pCell.get()->GetCellId();
          timers.m_generateCellIds.Pause();

          // Create cell from cell id
          timers.m_createCells.Start();
          std::unique_ptr < Model::Cell::ICell > pCellFromCellId = m_dggs.CreateCell(cellId);
          timers.m_createCells.Pause();

          // Convert cell back to a lat / long point
          timers.m_convertCells.Start();
          LatLong::SphericalAccuracyPoint returnedSphericalPoint = m_dggs.ConvertCellToLatLongPoint(
              *pCellFromCellId.get());
          timers.m_convertCells.Pause();

          timers.m_convertSphericalToWgs84.Start();
          const LatLong::Wgs84AccuracyPoint returnedPoint =
              coordinateConverter.ConvertSphereToWGS84(returnedSphericalPoint);
          timers.m_convertSphericalToWgs84.Pause();

          // Get the parents of the cell
          timers.m_getParentCells.Start();
          std::vector < std::unique_ptr<Model::Cell::ICell> > parents;
          m_dggs.GetParents(*pCellFromCellId, parents);
          timers.m_getParentCells.Pause();

          // Get the children of the cell
          timers.m_getChildCells.Start();
          std::vector < std::unique_ptr<Model::Cell::ICell> > children;
          m_dggs.GetChildren(*pCellFromCellId, children);
          timers.m_getChildCells.Pause();

          // Check the point we get back is close to the original point
          const double conversionError = originalSphericalPoint.GetDistanceToPoint(
              returnedSphericalPoint);
          ASSERT_LT(conversionError, conversionTolerance);

          // Check the accuracy we get back is close to the original accuracy
          // Tolerance is in degrees - convert metre squared accuracies to degrees for comparison
          const Degrees accuracyError = std::abs(
              EAGGR::LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(
                  originalPoint.GetAccuracy())
                  - EAGGR::LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(
                      returnedPoint.GetAccuracy()));
          ASSERT_LT(accuracyError, accuracyTolerance);

          // Update the averages for conversion and accuracy error
          conversionErrorAverage.Update(conversionError);
          accuracyErrorAverage.Update(accuracyError);

          // Update the count for the number of conversions completed
          ++noOfConversions;
        }
      }

      // Record the accuracy used at this level (as an area in metres squared)
      std::stringstream keyStream;
      keyStream << "AreaForAccuracyLevel" << accuracyLevel;
      std::stringstream valueStream;
      valueStream << accuracyArea;
      ::testing::Test::RecordProperty(keyStream.str(), valueStream.str());

      // Record the error averages at this accuracy
      StoreErrorValues("Conversion", accuracyLevel, conversionErrorAverage);
      StoreErrorValues("Accuracy", accuracyLevel, accuracyErrorAverage);
    }

    // Record the total number of conversions
    std::stringstream noOfConversionsStream;
    noOfConversionsStream << noOfConversions;
    ::testing::Test::RecordProperty("NumberOfConversions", noOfConversionsStream.str());

    // Record the total number of points on one face, two faces or multiple faces
    std::stringstream noOfOneFacePointsStream;
    std::stringstream noOfTwoFacePointsStream;
    std::stringstream noOfManyFacePointsStream;
    noOfOneFacePointsStream << noOfOneFacePoints;
    noOfTwoFacePointsStream << noOfTwoFacePoints;
    noOfManyFacePointsStream << noOfManyFacePoints;
    ::testing::Test::RecordProperty("PointsOnOneFace", noOfOneFacePointsStream.str());
    ::testing::Test::RecordProperty("PointsOnTwoFaces", noOfTwoFacePointsStream.str());
    ::testing::Test::RecordProperty("PointsOnManyFaces", noOfManyFacePointsStream.str());

    // Record the total times spent on each part of the conversion
    StoreTimerValues(timers);
  }

  Degrees Performance::CalculateLatitudeStep(
      const Degrees latitudeSpacing,
      const Degrees latLimitsMargin

      ) const
  {
    // Distance between the poles is half the circumference of the Earth
    static const double SPHERICAL_DIST_BETWEEN_POLES = PI * LatLong::Point::m_EARTH_RADIUS;

    // Calculate the step in latitude that will give the required spacing
    const Degrees latitudeStep = (latitudeSpacing / SPHERICAL_DIST_BETWEEN_POLES)
        * (LatLong::Point::m_MAX_LATITUDE - LatLong::Point::m_MIN_LATITUDE);

    // Calculate the number of degrees in the latitude range
    const Degrees latitudeRange = (LatLong::Point::m_MAX_LATITUDE - LatLong::Point::m_MIN_LATITUDE
        - (2.0 * latLimitsMargin));

    // Increase the latitude step so that rings of latitude points are evenly distributed
    const double noOfLatitudeSteps = floor(latitudeRange / latitudeStep);
    const Degrees adjustedLatitudeStep = latitudeRange / noOfLatitudeSteps;

    return (adjustedLatitudeStep);
  }

  void Performance::CalculateTolerances(
      const double accuracyArea,
      Degrees & conversionTolerance,
      Degrees & accuracyTolerance) const
  {
    // Tolerances are based on the worst case values
    static const double MAX_APERTURE = 4.0;

    // Worst case accuracy will be...
    // - When the point's accuracy is half way between the cell area of two resolutions
    static const double WORST_CASE_CELL_TO_ACCURACY_RATIO = 0.5 * MAX_APERTURE;
    accuracyTolerance = LatLong::SphericalAccuracyPoint::SquareMetresToAngleAccuracy(accuracyArea)
        / WORST_CASE_CELL_TO_ACCURACY_RATIO;

    // Worst case conversion will be...
    // - At the worst case accuracy
    // - On an equilateral triangle grid
    // - When the point is in the corner of the cell

    // Calculate cell area at worst case accuracy
    const double worstCaseCellArea = WORST_CASE_CELL_TO_ACCURACY_RATIO * accuracyArea;

    // Area of Triangular Cell = 0.5 * Edge Length * Height
    //                         = 0.5 * Edge Length * (sqrt(3) / 2 * Edge Length)
    //                         = (sqrt(3) / 4) * Edge Length ^ 2

    // Re-arrange to give Edge Length:
    const double edgeLength = sqrt((4 / sqrt(3)) * worstCaseCellArea);

    // Distance from Corner to Cell Centre:
    conversionTolerance = edgeLength / sqrt(3.0);

    // Cap the conversion tolerance to 0.01m - small accuracy areas
    if (conversionTolerance < 0.01)
    {
      conversionTolerance = 0.01;
    }

    // Cap the accuracy tolerance at 1E-6 degrees - smaller angles than this use the
    // small-angle approximation for the trigonometric functions so can be outside the tolerance
    if (accuracyTolerance < 1E-6)
    {
      accuracyTolerance = 1E-6;
    }
  }

  double Performance::CircumferenceAtLatitude(const Degrees a_latitude) const
  {
    // Equations taken from:
    // http://mathworld.wolfram.com/SphericalCap.html

    // Convert latitude to a positive angle in radians
    const Radians contactAngle = DEGREES_IN_RAD(std::abs(a_latitude));

    // Calculate vertical distance from nearest pole
    const double h = LatLong::Point::m_EARTH_RADIUS
        - (LatLong::Point::m_EARTH_RADIUS * sin(contactAngle));

    // Calculate radius of Earth at given latitude
    const double radius = sqrt(h * (2 * LatLong::Point::m_EARTH_RADIUS - h));

    // Calculate circumference using the radius
    const double circumference = 2 * PI * radius;

    return (circumference);
  }

  void Performance::StoreErrorValues(
      const std::string & a_errorName,
      const unsigned short a_accuracyLevel,
      const RunningAverage & a_average) const
  {
    std::stringstream keyStream;
    keyStream << "Maximum" << a_errorName << "ErrorAtAccuracyLevel" << a_accuracyLevel;
    std::stringstream valueStream;
    valueStream << a_average.GetMaximum();
    ::testing::Test::RecordProperty(keyStream.str(), valueStream.str());

    keyStream.str(std::string());
    keyStream << "Average" << a_errorName << "ErrorAtAccuracyLevel" << a_accuracyLevel;
    valueStream.str(std::string());
    valueStream << a_average.GetMeanAverage();
    ::testing::Test::RecordProperty(keyStream.str(), valueStream.str());

    keyStream.str(std::string());
    keyStream << "StdDeviationOf" << a_errorName << "ErrorAtAccuracyLevel" << a_accuracyLevel;
    valueStream.str(std::string());
    valueStream << a_average.GetStandardDeviation();
    ::testing::Test::RecordProperty(keyStream.str(), valueStream.str());
  }

  void Performance::StoreTimerValues(const Timers & a_timers) const
  {
    ::testing::Test::RecordProperty(
        "TimeSpentConvertingWgs84ToSpherical",
        static_cast<int>(a_timers.m_convertWgs84ToSpherical.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentConvertingSphericalToWgs84",
        static_cast<int>(a_timers.m_convertSphericalToWgs84.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentConvertingLatLongPoints",
        static_cast<int>(a_timers.m_convertLatLongPoints.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentGeneratingCellIds",
        static_cast<int>(a_timers.m_generateCellIds.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentCreatingCells",
        static_cast<int>(a_timers.m_createCells.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentConvertingCells",
        static_cast<int>(a_timers.m_convertCells.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentGettingParentCells",
        static_cast<int>(a_timers.m_getParentCells.GetTimeInMilliseconds()));
    ::testing::Test::RecordProperty(
        "TimeSpentGettingChildCells",
        static_cast<int>(a_timers.m_getChildCells.GetTimeInMilliseconds()));
  }
}
