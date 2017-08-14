//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file WktImporter.hpp
/// 
/// Implements the EAGGR::ImportExport::WktImporter class.
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

#include <string>
#include <vector>

#include "Src/ImportExport/AbstractShapeImporter.hpp"

namespace EAGGR
{
  namespace ImportExport
  {
    /// Class to import shape data from a WKT string.
    class WktImporter: public AbstractShapeImporter
    {
      public:
        /// Constructor
        /// @param a_wktString The WKT string to parse. Geometries must be defined in WGS84 coordinates.
        /// @param a_accuracy The angle defining the accuracy of the points in the WKT string in metres squared.
        WktImporter(const std::string& a_wktString, const double a_accuracy);
    };
  }
}
