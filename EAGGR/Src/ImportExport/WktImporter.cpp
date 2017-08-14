//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file WktImporter.cpp
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

#include <sstream>

#include "WktImporter.hpp"

#include "Src/EAGGRException.hpp"

using namespace EAGGR::LatLong;

namespace EAGGR
{
  namespace ImportExport
  {
    WktImporter::WktImporter(const std::string& a_wktString, const double a_accuracy)
        : AbstractShapeImporter(a_accuracy)
    {
      OGRGeometry* pGeometry;

      // Copy the WKT text - createFromWkt takes a non-const string
      std::vector<char> wktText(a_wktString.begin(), a_wktString.end());
      wktText.push_back('\0');
      char* wktChars = &wktText[0];

      // Create a geometry object from the WKT string
      OGRGeometryFactory::createFromWkt(&wktChars, NULL, &pGeometry);

      // If the WKT string was invalid throw exception
      if (pGeometry == NULL)
      {
        std::stringstream stream;
        stream << "Unsupported WKT: " << a_wktString << ". WKT string is not valid.";
        throw EAGGRException(stream.str());
      }

      // Get all of the supported shapes from the geometry
      ExtractShapesFromGeometry(pGeometry);

      delete pGeometry;
    }
  }
}

