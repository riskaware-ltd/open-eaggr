//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file TestTimer.cpp
/// 
/// Implements the TestTimer class.
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

#include <exception>

#include "TestTimer.hpp"

TestTimer::TestTimer()
    : isRunning(false), lastStart(0L), totalTime(0.0)
{
}

void TestTimer::Start()
{
  if (isRunning)
  {
    // Cannot start timer because it is already running
    throw std::exception();
  }

  isRunning = true;

  lastStart = std::clock();
}

void TestTimer::Pause()
{
  if (!isRunning)
  {
    // Cannot pause timer because it is not running
    throw std::exception();
  }

  isRunning = false;

  totalTime += (std::clock() - lastStart);
}

void TestTimer::Reset()
{
  isRunning = false;

  totalTime = 0.0;
}

double TestTimer::GetTimeInMilliseconds() const
{
  return (totalTime / static_cast<double>(CLOCKS_PER_SEC / 1000));
}
