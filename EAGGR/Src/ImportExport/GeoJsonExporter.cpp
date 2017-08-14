//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file GeoJsonExporter.cpp
/// 
/// Implements the EAGGR::ImportExport::GeoJsonExporter class.
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

#include "GeoJsonExporter.hpp"

#include "ogr_geometry.h"

#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::LatLong;

namespace EAGGR
{
  namespace ImportExport
  {
    std::string GeoJsonExporter::ExportShapes(std::vector<EAGGR::LatLong::LatLongShape> & a_shapes)
    {
      OGRGeometry* pGeometry;
      std::vector < OGRPoint > points;
      OGRMultiPoint multiPoint;

      OGRPolygon polygon;
      OGRLinearRing polygonRing;

      for (std::vector<EAGGR::LatLong::LatLongShape>::size_type shapeIndex = 0;
          shapeIndex != a_shapes.size(); shapeIndex++)
      {
        // Create the geometry implementation depending on the shape type
        switch (a_shapes[shapeIndex].GetShapeType())
        {
          case WGS84_ACCURACY_POINT:
          {
            const Wgs84AccuracyPoint* pPoint =
                static_cast<const Wgs84AccuracyPoint*>(a_shapes[shapeIndex].GetShapeData());

            points.push_back(OGRPoint(pPoint->GetLongitude(), pPoint->GetLatitude()));

            break;
          }
          case WGS84_POLYGON:
          {
            if (a_shapes.size() != 1)
            {
              throw EAGGRException("Exporter only supports one polygon at a time.");
            }

            const Wgs84Polygon* pPolygon =
                static_cast<const Wgs84Polygon*>(a_shapes[shapeIndex].GetShapeData());

            if (pPolygon->GetNumberOfInnerRings() > 0)
            {
              throw EAGGRException("Exporter only supports polygons with no inner rings.");
            }

            for (std::vector<Wgs84AccuracyPoint>::size_type pointIndex = 0;
                pointIndex != pPolygon->GetOuterRing()->GetNumberOfPoints(); pointIndex++)
            {
              const Wgs84AccuracyPoint* pPoint = pPolygon->GetOuterRing()->GetAccuracyPoint(
                  pointIndex);
              polygonRing.addPoint(pPoint->GetLongitude(), pPoint->GetLatitude());
            }

            polygon.addRing(&polygonRing);

            break;
          }
          default:
            throw EAGGRException("Unsupported shape in GeoJSON exporter.");
        }
      }

      // Set the geometry to be a POLYGON, POINT or MULTIPOINT depending on what shapes were supplied
      if (polygonRing.getNumPoints() > 0)
      {
        pGeometry = &polygon;
      }
      else if (a_shapes.size() == 1)
      {
        pGeometry = &points[0];
      }
      else
      {
        for (std::vector<EAGGR::LatLong::LatLongShape>::size_type shapeIndex = 0;
            shapeIndex != a_shapes.size(); shapeIndex++)
        {
          multiPoint.addGeometry(&(points[shapeIndex]));
        }

        pGeometry = &multiPoint;
      }

      // Export the GeoJSON
      char* geoJsonText = pGeometry->exportToJson();
      std::string geoJsonString(geoJsonText);

      // Clean up the variables
      OGRFree(geoJsonText);

      return geoJsonString;
    }
  }
}
