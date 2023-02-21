//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file KmlExporter.cpp
/// 
/// Implements the EAGGR::ImportExport::KmlExporter class.
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

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cmath>

#include "KmlExporter.hpp"

#include "Src/Utilities/Maths.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Cell;
using namespace EAGGR::LatLong;
using namespace EAGGR::Utilities;

namespace EAGGR
{
  namespace ImportExport
  {
    // Represents ~1cm accuracy
    static const int LAT_LON_NUM_DP = 7;
    static const Maths::Degrees HALF_MIN_INCREMENT = std::pow(10.0, -LAT_LON_NUM_DP) / 2.0;

    KmlExporter::KmlExporter(
        const EAGGR::Model::Projection::IProjection* const a_pProjection,
        const EAGGR::Model::GridIndexer::IGridIndexer* const a_pGridIndexer,
        const bool a_showCellCentre)
        :
            m_pProjection(a_pProjection),
            m_pGridIndexer(a_pGridIndexer),
            m_coordinateConverter(),
            m_showCellCentre(a_showCellCentre)
    {
    }

    void KmlExporter::AddLocation(
        const EAGGR::LatLong::Wgs84AccuracyPoint & a_point,
        const std::string& a_identifier)
    {
      Placemark placemark;
      placemark.name = a_identifier;
      placemark.latitude = a_point.GetLatitude();
      placemark.longitude = a_point.GetLongitude();

      m_locations.push_back(placemark);
    }

    void KmlExporter::AddCell(const EAGGR::Model::Cell::ICell & a_cell)
    {
      if (m_showCellCentre)
      {
        // Add a placemark at the cell centre
        FaceCoordinate centre = m_pGridIndexer->GetFaceCoordinate(a_cell);

        SphericalAccuracyPoint centrePoint = m_pProjection->GetLatLongPoint(centre);
        Wgs84AccuracyPoint wgs84CentrePoint = m_coordinateConverter.ConvertSphereToWGS84(
            centrePoint);

        Placemark placemark;
        placemark.name = a_cell.GetCellId();
        placemark.latitude = wgs84CentrePoint.GetLatitude();
        placemark.longitude = wgs84CentrePoint.GetLongitude();

        m_polygonCentres.push_back(placemark);
      }

      // Get the vertices and generate a polygon
      std::list < FaceCoordinate > vertices;
      m_pGridIndexer->GetCellVertices(a_cell, vertices);

      Polygon polygon;

      for (std::list<FaceCoordinate>::const_iterator iterator = vertices.begin();
          iterator != vertices.end(); ++iterator)
      {
        FaceCoordinate vertex = *iterator;

        SphericalAccuracyPoint vertexPoint = m_pProjection->GetLatLongPoint(vertex);
        Wgs84AccuracyPoint wgs84VertexPoint = m_coordinateConverter.ConvertSphereToWGS84(
            vertexPoint);

        polygon.vertices.push_back(wgs84VertexPoint);
      }

      // Close the polygon using the first point
      polygon.vertices.push_back(
          m_coordinateConverter.ConvertSphereToWGS84(
              m_pProjection->GetLatLongPoint(*(vertices.begin()))));

      m_polygons.push_back(polygon);
    }

    void KmlExporter::Export(std::ostream& a_outputStream) const
    {
      WriteHeader(a_outputStream);
      WriteLocations(a_outputStream);
      WritePolygonCentres(a_outputStream);
      WritePolygons(a_outputStream);
      WriteFooter(a_outputStream);
    }

    void KmlExporter::ExportToFile(const char * a_filename) const
    {
      /// @todo Check if file was created successfully.
      std::ofstream file;
      file.open(a_filename);
      Export(file);
      file.close();
    }

    void KmlExporter::WriteHeader(std::ostream& a_outputStream) const
    {
      // Output the xml header and style lines
      a_outputStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
      a_outputStream
          << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">"
          << std::endl;
      a_outputStream << "  <Document>" << std::endl;
      a_outputStream << "    <Style id=\"s_ylw-pushpin\">" << std::endl;
      a_outputStream << "      <IconStyle>" << std::endl;
      a_outputStream << "        <scale>1.1</scale>" << std::endl;
      a_outputStream << "        <Icon>" << std::endl;
      a_outputStream
          << "          <href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>"
          << std::endl;
      a_outputStream << "        </Icon>" << std::endl;
      a_outputStream << "        <hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>"
          << std::endl;
      a_outputStream << "      </IconStyle>" << std::endl;
      a_outputStream << "    </Style>" << std::endl;
      a_outputStream << "    <Style id=\"s_red-pushpin\">" << std::endl;
      a_outputStream << "      <IconStyle>" << std::endl;
      a_outputStream << "        <scale>1.1</scale>" << std::endl;
      a_outputStream << "        <Icon>" << std::endl;
      a_outputStream
          << "          <href>http://maps.google.com/mapfiles/kml/pushpin/red-pushpin.png</href>"
          << std::endl;
      a_outputStream << "        </Icon>" << std::endl;
      a_outputStream << "        <hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>"
          << std::endl;
      a_outputStream << "      </IconStyle>" << std::endl;
      a_outputStream << "    </Style>" << std::endl;
    }

    void KmlExporter::WriteLocations(std::ostream& a_outputStream) const
    {
      // Write each location that has been added to the exporter
      for (std::list<Placemark>::const_iterator iterator = m_locations.begin();
          iterator != m_locations.end(); ++iterator)
      {
        WritePlacemark(*iterator, "#s_red-pushpin", a_outputStream);
      }
    }

    void KmlExporter::WritePolygonCentres(std::ostream& a_outputStream) const
    {
      // Write each placemark that has been added to the exporter
      for (std::list<Placemark>::const_iterator iterator = m_polygonCentres.begin();
          iterator != m_polygonCentres.end(); ++iterator)
      {
        WritePlacemark(*iterator, "#s_ylw-pushpin", a_outputStream);
      }
    }

    void KmlExporter::WritePlacemark(
        const Placemark & a_placemark,
        const std::string & a_styleUrl,
        std::ostream& a_outputStream) const
    {
      a_outputStream << "    <Placemark>" << std::endl;
      a_outputStream << "      <name>" << a_placemark.name << "</name>" << std::endl;
      a_outputStream << "      <styleUrl>" << a_styleUrl << "</styleUrl>" << std::endl;
      a_outputStream << "      <Point>" << std::endl;
      a_outputStream << "        <gx:drawOrder>1</gx:drawOrder>" << std::endl;
      a_outputStream << std::fixed;
      a_outputStream << std::setprecision(LAT_LON_NUM_DP);
      a_outputStream << "        <coordinates>" << StandardiseLongitude(a_placemark.longitude)
          << "," << StandardiseLatitude(a_placemark.latitude) << ",0</coordinates>" << std::endl;
      a_outputStream << "      </Point>" << std::endl;
      a_outputStream << "    </Placemark>" << std::endl;
    }

    void KmlExporter::WritePolygons(std::ostream& a_outputStream) const
    {
      unsigned int polygonIndex = 1;

      // Write each polygon that has been added to the exporter
      for (std::list<Polygon>::const_iterator iterator = m_polygons.begin();
          iterator != m_polygons.end(); ++iterator)
      {
        Polygon polygon = *iterator;

        a_outputStream << "    <Placemark>" << std::endl;
        a_outputStream << "      <name>Polygon" << polygonIndex++ << "</name>" << std::endl;
        a_outputStream << "      <Style>" << std::endl;
        a_outputStream << "        <LineStyle>" << std::endl;
        a_outputStream << "          <color>ff0000ff</color>" << std::endl;
        a_outputStream << "          <width>5</width>" << std::endl;
        a_outputStream << "        </LineStyle>" << std::endl;
        a_outputStream << "        <PolyStyle>" << std::endl;
        a_outputStream << "          <color>00000000</color>" << std::endl;
        a_outputStream << "        </PolyStyle>" << std::endl;
        a_outputStream << "      </Style>" << std::endl;
        a_outputStream << "      <LineString>" << std::endl;
        a_outputStream << "        <tessellate>1</tessellate>" << std::endl;
        a_outputStream << "        <coordinates>" << std::endl;
        a_outputStream << "          ";
        a_outputStream << std::fixed;
        a_outputStream << std::setprecision(LAT_LON_NUM_DP);

        // Loop over each point in the polygon
        for (std::list<Wgs84AccuracyPoint>::const_iterator vertexIter = polygon.vertices.begin();
            vertexIter != polygon.vertices.end(); ++vertexIter)
        {
          Wgs84AccuracyPoint vertex = *vertexIter;

          a_outputStream << StandardiseLongitude(vertex.GetLongitude()) << ","
              << StandardiseLatitude(vertex.GetLatitude()) << ",0 ";
        }

        a_outputStream << std::endl;
        a_outputStream << "        </coordinates>" << std::endl;
        a_outputStream << "      </LineString>" << std::endl;
        a_outputStream << "    </Placemark>" << std::endl;
      }
    }

    void KmlExporter::WriteFooter(std::ostream& a_outputStream) const
    {
      // Close the tags from the header
      a_outputStream << "  </Document>" << std::endl;
      a_outputStream << "</kml>" << std::endl;
    }

    EAGGR::Utilities::Maths::Degrees KmlExporter::StandardiseLatitude(
        const EAGGR::Utilities::Maths::Degrees a_latitude) const
    {
      Maths::Degrees newLatitude = a_latitude;
      if (std::abs(newLatitude) < HALF_MIN_INCREMENT)
      {
        newLatitude = 0.0;
      }

      return newLatitude;
    }

    EAGGR::Utilities::Maths::Degrees KmlExporter::StandardiseLongitude(
        const EAGGR::Utilities::Maths::Degrees a_longitude) const
    {
      Maths::Degrees newLongitude = a_longitude;
      if (std::abs(newLongitude) < HALF_MIN_INCREMENT)
      {
        newLongitude = 0.0;
      }
      else if (newLongitude < Point::m_MIN_LONGITUDE + HALF_MIN_INCREMENT)
      {
        newLongitude = Point::m_MAX_LONGITUDE;
      }

      return newLongitude;
    }
  }
}
