//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file ICellExporter.hpp
/// 
/// Implements the EAGGR::ImportExport::ICellExporter class.
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

#include <ostream>
#include <list>

#include "Src/Model/ICell.hpp"
#include "Src/Model/IProjection.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"

namespace EAGGR
{
  namespace ImportExport
  {
    /// Interface for classes that export DGGS data.
    class ICellExporter
    {
      public:
        /// Destructor
        virtual ~ICellExporter()
        {
        }
        ;

        /// Adds a location to be exported.
        /// @param a_point The point to export.
        /// @param a_identifier An label identifier for the exported point.
        virtual void AddLocation(
            const EAGGR::LatLong::Wgs84AccuracyPoint & a_point,
            const std::string& a_identifier) = 0;

        /// Adds a cell to the exporter
        /// @param a_cell The cell to be added
        virtual void AddCell(const EAGGR::Model::Cell::ICell & a_cell) = 0;

        /// Exports the data that has been added to the exporter
        /// @param a_outputStream The stream to write the KML string to
        virtual void Export(std::ostream& a_outputStream) const = 0;

        /// Exports the data that has been added to the exporter to a file
        /// @param a_filename Path of the file the data will be exported to
        virtual void ExportToFile(const char * a_filename) const = 0;
    };
  }
}
