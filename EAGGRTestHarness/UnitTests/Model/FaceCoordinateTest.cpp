//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file FaceCoordinateTest.cpp
/// 
/// Tests for the EAGGR::Model::FaceCoordinate class.
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

#include "Src/Model/FaceCoordinate.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR;
using namespace EAGGR::Model;

UNIT_TEST(FaceCoordinate, Getters)
{
  FaceIndex faceIndex = 1U;
  double xOffset = 1.2345;
  double yOffset = 0.1234;
  double accuracy = 6.789;

  FaceCoordinate faceCoordinate(faceIndex, xOffset, yOffset, accuracy);

  EXPECT_EQ(faceIndex, faceCoordinate.GetFaceIndex());
  EXPECT_DOUBLE_EQ(xOffset, faceCoordinate.GetXOffset());
  EXPECT_DOUBLE_EQ(yOffset, faceCoordinate.GetYOffset());
  EXPECT_DOUBLE_EQ(accuracy, faceCoordinate.GetAccuracy());
}

UNIT_TEST(FaceCoordinate, InvalidConstruction)
{
  EXPECT_THROW(FaceCoordinate(1U, 1.2345, 0.1234, -6.789), EAGGRException);
}
