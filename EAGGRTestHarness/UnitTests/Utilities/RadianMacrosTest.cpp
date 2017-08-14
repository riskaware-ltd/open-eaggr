//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file RadianMacrosTest.cpp
///
/// Tests the macros contained in RadianMacros.hpp.
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

# include "Src/Utilities/RadianMacros.hpp"

UNIT_TEST(RadianMacros, DegreesInRad)
{
  EXPECT_NEAR(-1.5708, DEGREES_IN_RAD(-90.0), 0.0001);
  EXPECT_NEAR(-0.7854, DEGREES_IN_RAD(-45.0), 0.0001);
  EXPECT_NEAR(0.0000, DEGREES_IN_RAD(0.0), 0.0001);
  EXPECT_NEAR(0.7854, DEGREES_IN_RAD(45.0), 0.0001);
  EXPECT_NEAR(1.5708, DEGREES_IN_RAD(90.0), 0.0001);
  EXPECT_NEAR(2.3562, DEGREES_IN_RAD(135.0), 0.0001);
  EXPECT_NEAR(3.1416, DEGREES_IN_RAD(180.0), 0.0001);
  EXPECT_NEAR(6.2832, DEGREES_IN_RAD(360.0), 0.0001);
  EXPECT_NEAR(12.5664, DEGREES_IN_RAD(720.0), 0.0001);
}

UNIT_TEST(RadianMacros, RadiansInDeg)
{
  EXPECT_NEAR(-90.0, RADIANS_IN_DEG(-1.5708), 0.1);
  EXPECT_NEAR(-45.0, RADIANS_IN_DEG(-0.7854), 0.1);
  EXPECT_NEAR(0.0, RADIANS_IN_DEG(0.0), 0.1);
  EXPECT_NEAR(45.0, RADIANS_IN_DEG(0.7854), 0.1);
  EXPECT_NEAR(90.0, RADIANS_IN_DEG(1.5708), 0.1);
  EXPECT_NEAR(135.0, RADIANS_IN_DEG(2.3562), 0.1);
  EXPECT_NEAR(180.0, RADIANS_IN_DEG(3.1416), 0.1);
  EXPECT_NEAR(360.0, RADIANS_IN_DEG(6.2832), 0.1);
  EXPECT_NEAR(720.0, RADIANS_IN_DEG(12.5664), 0.1);
}
