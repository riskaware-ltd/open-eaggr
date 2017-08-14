//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
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
#define POSTGRES_TEST(TestBase, TestName) \
   TEST(PostgresTest##_##TestBase, TestName)

/// Used to disable tests that should not be run with the full test suite every time
#define DISABLED_POSTGRES_TEST(TestBase, TestName) \
   TEST(DISABLED_PostgresTest##_##TestBase, TestName)

static const char* CONNECTION_INFO = "dbname=dggs user=postgres password=postgres";
