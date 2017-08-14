//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file IShapeExporter.hpp
/// 
/// Implements the EAGGR::ImportExport::IShapeExporter class.
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

#include <vector>

#include "Src/LatLong/LatLongShape.hpp"

namespace EAGGR
{
  namespace ImportExport
  {
    /// Interface to export shape data
    class IShapeExporter
    {
      public:
        /// Destructor
        virtual ~IShapeExporter()
        {
        }
        ;

        /// Exports a shape object to a string.
        /// @param a_shapes The shapes to export.
        /// @return The string representation of the shape object.
        virtual std::string ExportShapes(std::vector<EAGGR::LatLong::LatLongShape> & a_shapes) = 0;
    };
  }
}
