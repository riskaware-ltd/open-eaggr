//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: System Tests
//
//------------------------------------------------------
/// @file BatchDggsTest.cpp
/// 
/// System Tests to input data from files, process through DGGS and output data to file
///
/// The tests in this file are designed for the user to be able to run DGGS on their own
/// data by populating the appropriate file.  The tests run using ISEA4T by default -
/// change the enum from DGGS_ISEA4T to DGGS_ISEA3H to switch between DGGS implementations.
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

#include <fstream>
#include <sstream>

#include "TestMacros.hpp"

#include "../EAGGR/API/eaggr_api.h"
#include "Src/EAGGRException.hpp"

SYSTEM_TEST(BatchDggsTest, ConvertPointsToDggsCells)
{
  static const unsigned short NO_OF_SHAPES = 1U;

  std::ifstream pointsFile;
  pointsFile.open("TestData/BatchDggsTest/Input/PointsToConvertToCells.csv");

  std::ofstream cellsFile;
  cellsFile.open("TestData/BatchDggsTest/Output/CellsCreatedFromPoints.csv");

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  // Open a connection to the DGGS library
  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  // Read line by line.
  // Lines should have the format latitude,longitude,accuracy(in metres squared)
  double latitude;
  char separator1;
  double longitude;
  char separator2;
  double accuracy;

  int line = 1;
  std::string fileLine;

  while (pointsFile >> fileLine)
  {
    std::stringstream lineStream(fileLine);
    lineStream >> latitude >> separator1 >> longitude >> separator2 >> accuracy;

    // If failed to read a line throw exception
    if (lineStream.fail())
    {
      std::stringstream errorStream;
      errorStream << "Invalid point data on line " << line;
      throw EAGGR::EAGGRException(errorStream.str());
    }

    // Create a DGGS shape from the point
    DGGS_LatLongShape latLongShapes[NO_OF_SHAPES];
    latLongShapes[0].m_type = DGGS_LAT_LONG_POINT;
    latLongShapes[0].m_data.m_point.m_latitude = latitude;
    latLongShapes[0].m_data.m_point.m_longitude = longitude;
    latLongShapes[0].m_data.m_point.m_accuracy = accuracy;

    // Convert the point to a DGGS cell
    DGGS_Shape * shapes;
    returnCode = EAGGR_ConvertShapesToDggsShapes(handle, latLongShapes, NO_OF_SHAPES, &shapes);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    // Output the cell
    cellsFile << shapes[0].m_data.m_cell<< std::endl;

    // Clean up the shape object
    returnCode = EAGGR_DeallocateDggsShapes(handle, &shapes, NO_OF_SHAPES);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    ++line;
  }

  pointsFile.close();
  cellsFile.close();

  // Close the DGGS library
  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(BatchDggsTest, ConvertDggsCellsToPoints)
{
  static const unsigned short NO_OF_CELLS = 1U;

  std::ifstream cellsFile;
  cellsFile.open("TestData/BatchDggsTest/Input/CellsToConvertToPoints.csv");

  std::ofstream pointsFile;
  pointsFile.open("TestData/BatchDggsTest/Output/PointsCreatedFromCells.csv");

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  // Open a connection to the DGGS library
  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  std::string cellId;

  int line = 1;

  // Loop over each line.  Each line should be a DGGS cell id.
  while (cellsFile >> cellId)
  {
    // If failed to read a line throw exception
    if (cellId.size() == 0)
    {
      std::stringstream errorStream;
      errorStream << "Invalid cell data on line " << line;
      throw EAGGR::EAGGRException(errorStream.str());
    }

    // Convert from the string read in to the DGGS cell
    DGGS_Cell dggsCellId[NO_OF_CELLS];
    static_cast<void>(strcpy(dggsCellId[0], cellId.c_str()));

    DGGS_LatLongPoint points[NO_OF_CELLS];

    // Convert the cell to a point
    returnCode = EAGGR_ConvertDggsCellsToPoints(handle, dggsCellId, NO_OF_CELLS, points);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    // Output the point
    pointsFile << points[0].m_latitude << "," << points[0].m_longitude << "," << points[0].m_accuracy << std::endl;

    ++line;
  }

  pointsFile.close();
  cellsFile.close();

  // Close the DGGS library
  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(BatchDggsTest, GetCellParents)
{
  static const unsigned short NO_OF_CELLS = 1U;

  std::ifstream cellsFile;
  cellsFile.open("TestData/BatchDggsTest/Input/CellsToGetParentsFor.csv");

  std::ofstream parentsFile;
  parentsFile.open("TestData/BatchDggsTest/Output/ParentCells.csv");

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  // Open a connection to the DGGS library
  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  std::string cellId;

  int line = 1;

  // Loop over each line.  Each line should be a DGGS cell id.
  while (cellsFile >> cellId)
  {
    // If failed to read a line throw exception
    if (cellId.size() == 0)
    {
      std::stringstream errorStream;
      errorStream << "Invalid point data on line " << line;
      throw EAGGR::EAGGRException(errorStream.str());
    }

    // Convert from the string read in to the DGGS cell
    DGGS_Cell dggsCellId[NO_OF_CELLS];
    static_cast<void>(strcpy(dggsCellId[0], cellId.c_str()));

    DGGS_Cell parentCells[EAGGR_MAX_PARENT_CELLS] =
    {};
    unsigned short noOfParentCells = 0U;

    // Get the parent cells
    returnCode = EAGGR_GetDggsCellParents(handle, dggsCellId[0], parentCells, &noOfParentCells);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    // Write out the parent cells
    for (unsigned short parent = 0; parent < noOfParentCells; ++parent)
    {
      parentsFile << parentCells[parent] << ",";
    }

    parentsFile << std::endl;

    ++line;
  }

  parentsFile.close();
  cellsFile.close();

  // Close the DGGS library
  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}

SYSTEM_TEST(BatchDggsTest, GetCellChildren)
{
  static const unsigned short NO_OF_CELLS = 1U;

  std::ifstream cellsFile;
  cellsFile.open("TestData/BatchDggsTest/Input/CellsToGetChildrenFor.csv");

  std::ofstream childrenFile;
  childrenFile.open("TestData/BatchDggsTest/Output/ChildCells.csv");

  DGGS_Handle handle = NULL;
  DGGS_ReturnCode returnCode;

  // Open a connection to the DGGS library
  returnCode = EAGGR_OpenDggsHandle(DGGS_ISEA4T, &handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);

  std::string cellId;

  int line = 1;

  // Loop over each line.  Each line should be a DGGS cell id.
  while (cellsFile >> cellId)
  {
    // If failed to read a line throw exception
    if (cellId.size() == 0)
    {
      std::stringstream errorStream;
      errorStream << "Invalid point data on line " << line;
      throw EAGGR::EAGGRException(errorStream.str());
    }

    // Convert from the string read in to the DGGS cell
    DGGS_Cell dggsCellId[NO_OF_CELLS];
    static_cast<void>(strcpy(dggsCellId[0], cellId.c_str()));

    DGGS_Cell childCells[EAGGR_MAX_CHILD_CELLS] =
    {};
    unsigned short noOfChildCells = 0U;

    // Get the child cells
    returnCode = EAGGR_GetDggsCellChildren(handle, dggsCellId[0], childCells, &noOfChildCells);
    ASSERT_EQ(DGGS_SUCCESS, returnCode);

    // Write out the child cells
    for (unsigned short child = 0; child < noOfChildCells; ++child)
    {
      childrenFile << childCells[child] << ",";
    }

    childrenFile << std::endl;

    ++line;
  }

  childrenFile.close();
  cellsFile.close();

  // Close the DGGS library
  returnCode = EAGGR_CloseDggsHandle(&handle);
  ASSERT_EQ(DGGS_SUCCESS, returnCode);
}
