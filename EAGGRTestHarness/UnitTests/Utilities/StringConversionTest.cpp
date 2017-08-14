//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file StringConversionTest.cpp
/// 
/// Tests for functions in the EAGGR::Utilities::StringConversion namespace.
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

#include "Src/Utilities/StringConversion.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::StringConversion;

UNIT_TEST(StringConversion, StringToBase10UnsignedShort)
{
  unsigned short output;
  ConversionResult result;

  result = StringToBase10UnsignedShort("001", output);
  ASSERT_EQ(CONVERSION_SUCCESSFUL, result);
  EXPECT_EQ(output, 1);

  result = StringToBase10UnsignedShort("100", output);
  ASSERT_EQ(CONVERSION_SUCCESSFUL, result);
  EXPECT_EQ(output, 100);

  result = StringToBase10UnsignedShort("XXX", output);
  ASSERT_EQ(CONVERSION_INCONVERTIBLE, result);

  result = StringToBase10UnsignedShort("1e10", output);
  ASSERT_EQ(CONVERSION_INCONVERTIBLE, result);

  result = StringToBase10UnsignedShort("-1", output);
  ASSERT_EQ(CONVERSION_OUT_OF_RANGE, result);

  result = StringToBase10UnsignedShort("1000000", output);
  ASSERT_EQ(CONVERSION_OUT_OF_RANGE, result);
}

UNIT_TEST(StringConversion, StringToBase10Short)
{
  short output;
  ConversionResult result;

  result = StringToBase10Short("001", output);
  ASSERT_EQ(CONVERSION_SUCCESSFUL, result);
  EXPECT_EQ(output, 1);

  result = StringToBase10Short("-10", output);
  ASSERT_EQ(CONVERSION_SUCCESSFUL, result);
  EXPECT_EQ(output, -10);

  result = StringToBase10Short("XXX", output);
  ASSERT_EQ(CONVERSION_INCONVERTIBLE, result);

  result = StringToBase10Short("-1e10", output);
  ASSERT_EQ(CONVERSION_INCONVERTIBLE, result);

  result = StringToBase10Short("-1000000", output);
  ASSERT_EQ(CONVERSION_OUT_OF_RANGE, result);

  result = StringToBase10Short("1000000", output);
  ASSERT_EQ(CONVERSION_OUT_OF_RANGE, result);
}
