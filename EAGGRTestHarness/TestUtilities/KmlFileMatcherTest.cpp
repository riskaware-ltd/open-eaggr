//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file KmlFileMatcherTest.cpp
/// 
/// Unit tests for the KmlFileMatcher class.
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

#include <fstream>

#include "KmlFileMatcher.hpp"

using namespace EAGGR::TestUtilities;

UNIT_TEST(KmlFileMatcher, MatchingFiles)
{
  std::ifstream stream1("../EAGGRTestHarness/TestData/KmlMatcherTest/BaselineFile.kml");
  std::ifstream stream2("../EAGGRTestHarness/TestData/KmlMatcherTest/BaselineFile.kml");

  KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(stream1, stream2);

  stream1.close();
  stream2.close();

  EXPECT_TRUE(filesMatch);
}

UNIT_TEST(KmlFileMatcher, NonMatchingFiles)
{
  std::ifstream stream1("../EAGGRTestHarness/TestData/KmlMatcherTest/BaselineFile.kml");
  std::ifstream stream2("../EAGGRTestHarness/TestData/KmlMatcherTest/NonMatchingFile.kml");

  KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(stream1, stream2);

  stream1.close();
  stream2.close();

  EXPECT_FALSE(filesMatch);
}

UNIT_TEST(KmlFileMatcher, AdditionalLineFile)
{
  std::ifstream stream1("../EAGGRTestHarness/TestData/KmlMatcherTest/BaselineFile.kml");
  std::ifstream stream2("../EAGGRTestHarness/TestData/KmlMatcherTest/AdditionalLineFile.kml");

  KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(stream1, stream2);

  stream1.close();
  stream2.close();

  EXPECT_FALSE(filesMatch);
}

UNIT_TEST(KmlFileMatcher, FewerLinesFile)
{
  std::ifstream stream1("../EAGGRTestHarness/TestData/KmlMatcherTest/BaselineFile.kml");
  std::ifstream stream2("../EAGGRTestHarness/TestData/KmlMatcherTest/FewerLinesFile.kml");

  KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(stream1, stream2);

  stream1.close();
  stream2.close();

  EXPECT_FALSE(filesMatch);
}

UNIT_TEST(KmlFileMatcher, EmptyStreams)
{
  std::istringstream stream1;
  std::istringstream stream2;

  KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(stream1, stream2);

  EXPECT_TRUE(filesMatch);
}
