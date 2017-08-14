//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file IcosahedronTest.cpp
/// 
/// Tests for the EAGGR::Model::Icosahedron class.
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

#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/EAGGRException.hpp"
#include "Src/Utilities/RadianMacros.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::PolyhedralGlobe;

UNIT_TEST(Icosahedron, Getters)
{
  Icosahedron icosahedron;

  EXPECT_EQ(20U, icosahedron.GetNoOfFaces());
  EXPECT_DOUBLE_EQ(DEGREES_IN_RAD(37.37736814), icosahedron.Get_g());
  EXPECT_DOUBLE_EQ(DEGREES_IN_RAD(36.0), icosahedron.Get_G());
  EXPECT_DOUBLE_EQ(DEGREES_IN_RAD(30.0), icosahedron.GetTheta());
  EXPECT_DOUBLE_EQ(0.91038328153090290025, icosahedron.GetRPrimeRelativeToR());
  EXPECT_DOUBLE_EQ(1.323169076499215, icosahedron.GetEdgeLengthRelativeToRPrime());
}

UNIT_TEST(Icosahedron, FaceOrientation)
{
  Icosahedron icosahedron;

  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(0U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(1U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(2U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(3U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(4U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(5U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(6U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(7U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(8U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(9U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(10U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(11U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(12U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(13U));
  EXPECT_EQ(0.0, icosahedron.GetOrientationOfFace(14U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(15U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(16U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(17U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(18U));
  EXPECT_EQ(DEGREES_IN_RAD(60.0), icosahedron.GetOrientationOfFace(19U));

  EXPECT_THROW(icosahedron.GetOrientationOfFace(20U), EAGGR::EAGGRException);
}

UNIT_TEST(Icosahedron, FaceCentre)
{
  Icosahedron icosahedron;

  EXPECT_EQ(52.62263186, icosahedron.GetFaceCentre(0U).GetLatitude());
  EXPECT_EQ(-144.0, icosahedron.GetFaceCentre(0U).GetLongitude());

  EXPECT_EQ(52.62263186, icosahedron.GetFaceCentre(1U).GetLatitude());
  EXPECT_EQ(-72.0, icosahedron.GetFaceCentre(1U).GetLongitude());

  EXPECT_EQ(52.62263186, icosahedron.GetFaceCentre(2U).GetLatitude());
  EXPECT_EQ(0.0, icosahedron.GetFaceCentre(2U).GetLongitude());

  EXPECT_EQ(52.62263186, icosahedron.GetFaceCentre(3U).GetLatitude());
  EXPECT_EQ(72.0, icosahedron.GetFaceCentre(3U).GetLongitude());

  EXPECT_EQ(52.62263186, icosahedron.GetFaceCentre(4U).GetLatitude());
  EXPECT_EQ(144.0, icosahedron.GetFaceCentre(4U).GetLongitude());

  EXPECT_EQ(10.81231696, icosahedron.GetFaceCentre(5U).GetLatitude());
  EXPECT_EQ(-144.0, icosahedron.GetFaceCentre(5U).GetLongitude());

  EXPECT_EQ(10.81231696, icosahedron.GetFaceCentre(6U).GetLatitude());
  EXPECT_EQ(-72.0, icosahedron.GetFaceCentre(6U).GetLongitude());

  EXPECT_EQ(10.81231696, icosahedron.GetFaceCentre(7U).GetLatitude());
  EXPECT_EQ(0.0, icosahedron.GetFaceCentre(7U).GetLongitude());

  EXPECT_EQ(10.81231696, icosahedron.GetFaceCentre(8U).GetLatitude());
  EXPECT_EQ(72.0, icosahedron.GetFaceCentre(8U).GetLongitude());

  EXPECT_EQ(10.81231696, icosahedron.GetFaceCentre(9U).GetLatitude());
  EXPECT_EQ(144.0, icosahedron.GetFaceCentre(9U).GetLongitude());

  EXPECT_EQ(-10.81231696, icosahedron.GetFaceCentre(10U).GetLatitude());
  EXPECT_EQ(-108.0, icosahedron.GetFaceCentre(10U).GetLongitude());

  EXPECT_EQ(-10.81231696, icosahedron.GetFaceCentre(11U).GetLatitude());
  EXPECT_EQ(-36.0, icosahedron.GetFaceCentre(11U).GetLongitude());

  EXPECT_EQ(-10.81231696, icosahedron.GetFaceCentre(12U).GetLatitude());
  EXPECT_EQ(36.0, icosahedron.GetFaceCentre(12U).GetLongitude());

  EXPECT_EQ(-10.81231696, icosahedron.GetFaceCentre(13U).GetLatitude());
  EXPECT_EQ(108.0, icosahedron.GetFaceCentre(13U).GetLongitude());

  EXPECT_EQ(-10.81231696, icosahedron.GetFaceCentre(14U).GetLatitude());
  EXPECT_EQ(180.0, icosahedron.GetFaceCentre(14U).GetLongitude());

  EXPECT_EQ(-52.62263186, icosahedron.GetFaceCentre(15U).GetLatitude());
  EXPECT_EQ(-108.0, icosahedron.GetFaceCentre(15U).GetLongitude());

  EXPECT_EQ(-52.62263186, icosahedron.GetFaceCentre(16U).GetLatitude());
  EXPECT_EQ(-36.0, icosahedron.GetFaceCentre(16U).GetLongitude());

  EXPECT_EQ(-52.62263186, icosahedron.GetFaceCentre(17U).GetLatitude());
  EXPECT_EQ(36.0, icosahedron.GetFaceCentre(17U).GetLongitude());

  EXPECT_EQ(-52.62263186, icosahedron.GetFaceCentre(18U).GetLatitude());
  EXPECT_EQ(108.0, icosahedron.GetFaceCentre(18U).GetLongitude());

  EXPECT_EQ(-52.62263186, icosahedron.GetFaceCentre(19U).GetLatitude());
  EXPECT_EQ(180.0, icosahedron.GetFaceCentre(19U).GetLongitude());

  EXPECT_THROW(icosahedron.GetFaceCentre(20U), EAGGR::EAGGRException);
}
