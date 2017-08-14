//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file GeoJsonImporter.cpp
/// 
/// Implements the EAGGR::ImportExport::GeoJsonImporter class.
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

#include "GeoJsonImporter.hpp"

#include "ogrgeojsonreader.h"

#include "Src/EAGGRException.hpp"

using namespace EAGGR::LatLong;

namespace EAGGR
{
  namespace ImportExport
  {
    GeoJsonImporter::GeoJsonImporter(const std::string& a_geoJsonString, const double a_accuracy)
        : AbstractShapeImporter(a_accuracy)
    {
      OGRGeometry* pGeometry;

      OGRGeoJSONReader reader;
      reader.Parse(a_geoJsonString.c_str());

      pGeometry = OGRGeoJSONReadGeometry(reader.GetJSonObject());

      // If the GeoJSON string was invalid throw exception
      if (pGeometry == NULL)
      {
        std::stringstream stream;
        stream << "Unsupported GeoJSON: " << a_geoJsonString << ". GeoJSON string is not valid.";
        throw EAGGRException(stream.str());
      }

      // Get all of the supported shapes from the geometry
      ExtractShapesFromGeometry(pGeometry);

      delete pGeometry;
    }
  }
}

