//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file CellPartitionTest.cpp
///
/// Tests the EAGGR::Model::Grid::CellPartition class
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

#include "Src/Model/IGrid/CellPartition.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Grid;

UNIT_TEST(CellPartition, Getters)
{
  unsigned short partitionId = 1;
  CartesianPoint partitionCentre(0.3, 0.4);
  ShapeOrientation orientation = ROTATED;

  CellPartition partition(partitionId, partitionCentre, orientation);

  EXPECT_EQ(partitionId, partition.GetId());
  EXPECT_EQ(partitionCentre.GetX(), partition.GetPartitionCentre().GetX());
  EXPECT_EQ(partitionCentre.GetY(), partition.GetPartitionCentre().GetY());
  EXPECT_EQ(orientation, partition.GetPartitionOrientation());
}

UNIT_TEST(CellPartition, Setters)
{
  unsigned short partitionId = 1;
  CartesianPoint partitionCentre(0.3, 0.4);
  ShapeOrientation orientation = ROTATED;

  CellPartition partition(2, CartesianPoint(0.0, 0.0), STANDARD);

  partition.SetId(partitionId);
  partition.SetPartitionCentre(partitionCentre);
  partition.SetPartitionOrientation(orientation);

  EXPECT_EQ(partitionId, partition.GetId());
  EXPECT_EQ(partitionCentre.GetX(), partition.GetPartitionCentre().GetX());
  EXPECT_EQ(partitionCentre.GetY(), partition.GetPartitionCentre().GetY());
  EXPECT_EQ(orientation, partition.GetPartitionOrientation());
}
