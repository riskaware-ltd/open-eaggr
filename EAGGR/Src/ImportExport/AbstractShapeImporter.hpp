//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file AbstractShapeImporter.hpp
///
/// Implements the EAGGR::ImportExport::AbstractShapeImporter class.
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

#include "Src/LatLong/LatLongShape.hpp"

#include "ogr_geometry.h"

namespace EAGGR
{
  namespace ImportExport
  {
    /// Base class for shape data importers
    class AbstractShapeImporter
    {
      public:
        /// Set the accuracy for the shapes and initialise the shape iterator to zero.
        /// @param a_accuracy The angle defining the accuracy of the points in metres squared.
        AbstractShapeImporter(const double a_accuracy);

        /// Frees all memory allocated to the shape objects.
        virtual ~AbstractShapeImporter();

        /// @return The next available shape object.
        virtual EAGGR::LatLong::LatLongShape GetNextShape();

        /// @return True if there are more shapes available; false otherwise.
        virtual bool HasNext() const;

      protected:
        /// Extracts all of the supported shapes from the supplied geometry.
        /// @param a_pGeometry The geometry to extract shapes from.
        void ExtractShapesFromGeometry(const OGRGeometry* const a_pGeometry);

      private:
        std::vector<EAGGR::LatLong::LatLongShape> m_shapes;
        unsigned short m_shapeIndex;
        const double m_accuracy;
    };
  }
}
