//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file LatLongShapeTest.cpp
/// 
/// Tests for the EAGGR::LatLong::LatLongShape class.
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

#include "Src/LatLong/LatLongShape.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"

using namespace EAGGR::LatLong;

UNIT_TEST(LatLongShape, Getters)
{
  Wgs84AccuracyPoint point(1.234, 2.345, 1E-4);
  LatLongShape shape(WGS84_ACCURACY_POINT, &point);

  EXPECT_EQ(WGS84_ACCURACY_POINT, shape.GetShapeType());
  EXPECT_EQ(&point, shape.GetShapeData());
}
