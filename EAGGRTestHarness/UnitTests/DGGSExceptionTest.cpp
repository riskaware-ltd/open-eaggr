//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file EAGGRExceptionTest.cpp
/// 
/// Tests for the EAGGR::EAGGRException class.
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

#include "Src/EAGGRException.hpp"

using namespace EAGGR;

UNIT_TEST(EAGGRException, Getters)
{
  std::string message = "This is an EAGGR Exception Message.";
  std::string expectedMessage = "EAGGR Exception: This is an EAGGR Exception Message.";
  EAGGRException exception(message);

  ASSERT_EQ(expectedMessage, exception.what());
}
