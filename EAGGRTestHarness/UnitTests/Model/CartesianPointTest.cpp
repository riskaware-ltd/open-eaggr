//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file CartesianPointTest.cpp
/// 
/// Tests for the EAGGR::Model::CartesianPoint class.
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

#include "Src/Model/CartesianPoint.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR;
using namespace EAGGR::Model;

UNIT_TEST(CartesianPoint, Constructor)
{
  double x = 1.2345;
  double y = 2.345;

  CartesianPoint point(x, y);

  EXPECT_DOUBLE_EQ(x, point.GetX());
  EXPECT_DOUBLE_EQ(y, point.GetY());
}

UNIT_TEST(CartesianPoint, Setters)
{
  double x = 1.2345;
  double y = 2.345;

  CartesianPoint point(0.0, 0.0);
  point.SetX(x);
  point.SetY(y);

  EXPECT_DOUBLE_EQ(x, point.GetX());
  EXPECT_DOUBLE_EQ(y, point.GetY());
}

UNIT_TEST(CartesianPoint, GetDistanceToPoint)
{
  CartesianPoint point(1.0, 2.0);

  EXPECT_DOUBLE_EQ(5.0, point.GetDistanceToPoint(CartesianPoint(4.0, 6.0)));
}
