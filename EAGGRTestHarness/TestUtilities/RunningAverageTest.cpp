//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file RunningAverageTest.cpp
/// 
/// Unit tests for the RunningAverage class.
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

#include "TestUtilities/RunningAverage.hpp"

UNIT_TEST(RunningAverage, AveragingTenPoints)
{
  static const double TOLERANCE = 0.01;

  static const double DATA[] =
  { 5.0, 3.0, 5.5, 4.0, 5.0, 6.0, 5.5, 4.5, 5.0, 1.0};
  static const unsigned short NO_OF_DATA_POINTS = sizeof(DATA) / sizeof(*DATA);

  RunningAverage average;
  for (unsigned short dataIndex = 0U; dataIndex < NO_OF_DATA_POINTS; dataIndex++)
  {
    average.Update(DATA[dataIndex]);
  }

  // Test data generated using the Microsoft Excel 2010 functions: MAX(), AVERAGE() and STDEV.P()
  EXPECT_NEAR(6.00, average.GetMaximum(), TOLERANCE);
  EXPECT_NEAR(4.45, average.GetMeanAverage(), TOLERANCE);
  EXPECT_NEAR(1.40, average.GetStandardDeviation(), TOLERANCE);
}
