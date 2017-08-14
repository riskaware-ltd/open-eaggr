//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file MathsTest.cpp
/// 
/// Tests for the EAGGR::Utilities::Maths class.
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

#include <limits>

#include "TestMacros.hpp"

#include "Src/Utilities/Maths.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

static const double TOLERANCE = 1e-6;

UNIT_TEST(Maths, Cot)
{
  // Cot is infinite at an angle of zero
  EXPECT_EQ(std::numeric_limits<double>::infinity(), Cot(DEGREES_IN_RAD(0.0)));

  EXPECT_NEAR(1.732051, Cot(DEGREES_IN_RAD(30.0)), TOLERANCE);
  EXPECT_NEAR(0.577350, Cot(DEGREES_IN_RAD(60.0)), TOLERANCE);
  EXPECT_NEAR(0.0, Cot(DEGREES_IN_RAD(90.0)), TOLERANCE);
  EXPECT_NEAR(-0.577350, Cot(DEGREES_IN_RAD(120.0)), TOLERANCE);
  EXPECT_NEAR(-1.732051, Cot(DEGREES_IN_RAD(150.0)), TOLERANCE);
  EXPECT_NEAR(-57295.779507, Cot(DEGREES_IN_RAD(179.999)), TOLERANCE);

  // Cot is -infinity at an angle of 180 degrees.
  // Due to numerical accuracy of PI it isn't actually infinite
  EXPECT_LT(Cot(DEGREES_IN_RAD(180.0)), -8.0e15);

  EXPECT_NEAR(57295.779507, Cot(DEGREES_IN_RAD(180.001)), TOLERANCE);
  EXPECT_NEAR(1.732051, Cot(DEGREES_IN_RAD(210.0)), TOLERANCE);
  EXPECT_NEAR(0.577350, Cot(DEGREES_IN_RAD(240.0)), TOLERANCE);
  EXPECT_NEAR(0.0, Cot(DEGREES_IN_RAD(270.0)), TOLERANCE);
  EXPECT_NEAR(-0.577350, Cot(DEGREES_IN_RAD(300.0)), TOLERANCE);
  EXPECT_NEAR(-1.732051, Cot(DEGREES_IN_RAD(330.0)), TOLERANCE);
  EXPECT_NEAR(-57295.779507265, Cot(DEGREES_IN_RAD(359.999)), TOLERANCE);
}

UNIT_TEST(Maths, Squared)
{
  EXPECT_NEAR(0.0, Squared(0.0), TOLERANCE);
  EXPECT_NEAR(1.0, Squared(1.0), TOLERANCE);
  EXPECT_NEAR(4.0, Squared(2.0), TOLERANCE);
  EXPECT_NEAR(9.0, Squared(3.0), TOLERANCE);
  EXPECT_NEAR(2.25, Squared(1.5), TOLERANCE);
}

UNIT_TEST(Maths, GetRadiusFromCircleArea)
{
  EXPECT_NEAR(0.0, GetRadiusFromCircleArea(0.0), TOLERANCE);
  EXPECT_NEAR(1.0, GetRadiusFromCircleArea(PI), TOLERANCE);
  EXPECT_NEAR(6.268753, GetRadiusFromCircleArea(123.456), TOLERANCE);

  EXPECT_THROW(GetRadiusFromCircleArea(-1.0), EAGGR::EAGGRException);
}
