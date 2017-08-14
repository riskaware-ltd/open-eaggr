//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file KmlTestCell.hpp
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

#pragma once

#include <list>

#include "Src/Model/ICell.hpp"

namespace EAGGR
{
  namespace TestUtilities
  {
    /// Implementation of ICell that provides functionality for KML testing
    class KmlTestCell: public EAGGR::Model::Cell::ICell
    {
      public:
        /// Constructor
        /// @param a_cellId The value to be returned by GetCellId()
        KmlTestCell(const Model::Cell::DggsCellId a_cellId);

        virtual Model::Cell::DggsCellId GetCellId() const;

        virtual unsigned short GetFaceIndex() const;
        virtual unsigned short GetResolution() const;
        virtual EAGGR::Model::Grid::ShapeOrientation GetOrientation() const;
        virtual void SetOrientation(const EAGGR::Model::Grid::ShapeOrientation a_orientation);
        virtual EAGGR::Model::Cell::CellLocation GetCellLocation() const;

      private:
        const Model::Cell::DggsCellId m_cellId;
    };
  }
}
