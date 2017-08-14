//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: System Tests
//
//------------------------------------------------------
/// @file MultithreadTest.cpp
/// 
/// Tests the DGGS DLL in a multithreaded environment.
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

#include <cstdio>
#include <fstream>
#include <thread>

#include "TestMacros.hpp"

#include "../EAGGR/API/eaggr_api.h"

void ConvertPoint(DGGS_LatLongPoint point)
{
  try
  {
    DGGS_Handle handle = NULL;
    DGGS_ReturnCode returnCode;

    returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    DGGS_Cell cell;
    returnCode = EAGGR_ConvertPointsToDggsCells(handle, &point, 1, &cell);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    DGGS_LatLongPoint convertedPoint;
    returnCode = EAGGR_ConvertDggsCellsToPoints(handle, &cell, 1, &convertedPoint);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    ASSERT_NEAR(point.m_latitude, convertedPoint.m_latitude, 1E-7);
    ASSERT_NEAR(point.m_longitude, convertedPoint.m_longitude, 1E-7);

    returnCode = EAGGR_CloseDggsHandle(&handle);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);
  }
  catch (...)
  {
    FAIL() << "An exception was thrown when converting point on a thread.";
  }
}

SYSTEM_TEST(Multithread, ConvertPoints)
{
  std::vector<std::thread> threads;

  for (double latitude = -80; latitude < 80; latitude += 10)
  {
    for (double longitude = -170; longitude <= 170; longitude += 10)
    {
      DGGS_LatLongPoint point;
      point.m_latitude = latitude;
      point.m_longitude = longitude;
      point.m_accuracy = 1E-5;

      threads.push_back(std::thread(ConvertPoint, point));
    }
  }

  for (std::vector<std::thread>::iterator iter = threads.begin(); iter != threads.end(); ++iter)
  {
    iter->join();
  }
}
