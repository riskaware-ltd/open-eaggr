//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: N/A
//
//------------------------------------------------------
/// @file TestMacros.hpp
/// 
/// Macros for creating a test.
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

#include "gtest/gtest.h"

#pragma once

/// Used to group together all the unit tests.
#define UNIT_TEST(TestBase, TestName) \
   TEST(UnitTest##_##TestBase, TestName)

/// Used to group together all the system tests.
#define SYSTEM_TEST(TestBase, TestName) \
   TEST(SystemTest##_##TestBase, TestName)

/// Used to group together all the evaluation tests.
#define EVALUATION_TEST(TestBase, TestName) \
   TEST(EvaluationTest##_##TestBase, TestName)
