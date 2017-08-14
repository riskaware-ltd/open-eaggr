//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file KmlTestCell.cpp
/// 
/// Implements the EAGGR::TestUtilities::KmlTestCell class for use in KML testing
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

#include <TestUtilities/KmlTestCell.hpp>

namespace EAGGR
{
  namespace TestUtilities
  {
    KmlTestCell::KmlTestCell(const Model::Cell::DggsCellId a_cellId)
        : m_cellId(a_cellId)
    {
    }

    Model::Cell::DggsCellId KmlTestCell::GetCellId() const
    {
      return m_cellId;
    }

    unsigned short KmlTestCell::GetFaceIndex() const
    {
      // Not used by the tests
      return 0U;
    }

    unsigned short KmlTestCell::GetResolution() const
    {
      // Not used by the tests
      return 0U;
    }

    EAGGR::Model::Grid::ShapeOrientation KmlTestCell::GetOrientation() const
    {
      // Not used by the tests
      return EAGGR::Model::Grid::STANDARD;
    }

    void KmlTestCell::SetOrientation(const EAGGR::Model::Grid::ShapeOrientation a_orientation)
    {
      // Not used by the tests
    }

    EAGGR::Model::Cell::CellLocation KmlTestCell::GetCellLocation() const
    {
      // Not used by the tests
      return EAGGR::Model::Cell::UNKNOWN;
    }
  }
}
