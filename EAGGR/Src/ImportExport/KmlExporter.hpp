//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file KmlExporter.hpp
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

#pragma once

#include <ostream>
#include <list>

#include "Src/ImportExport/ICellExporter.hpp"
#include "Src/Model/ICell.hpp"
#include "Src/Model/IProjection.hpp"
#include "Src/Model/IGridIndexer.hpp"
#include "Src/CoordinateConversion/CoordinateConverter.hpp"

namespace EAGGR
{
  namespace ImportExport
  {
    /// Class to export DGGS data to KML for visualisation
    class KmlExporter: public ICellExporter
    {
      public:
        /// Constructor
        /// @param a_pProjection The projection to project DGGS points back to Earth coordinates.
        /// @param a_pGridIndexer The grid indexer to get the cell coordinates from.
        /// @param a_showCellCentre Flag to set whether cell centre markers are shown.  Defaults to true.
        KmlExporter(
            const EAGGR::Model::Projection::IProjection* const a_pProjection,
            const EAGGR::Model::GridIndexer::IGridIndexer* const a_pGridIndexer,
            const bool a_showCellCentre = true);

        virtual void AddLocation(
            const EAGGR::LatLong::Wgs84AccuracyPoint & a_point,
            const std::string& a_identifier);

        virtual void AddCell(const EAGGR::Model::Cell::ICell & a_cell);

        virtual void Export(std::ostream& a_outputStream) const;
        virtual void ExportToFile(const char * a_filename) const;

      private:
        /// Data required to write out placemarks.  Lat/long in degrees.
        struct Placemark
        {
            std::string name;
            EAGGR::Utilities::Maths::Degrees latitude;
            EAGGR::Utilities::Maths::Degrees longitude;
        };

        /// Polygon defined by a set of vertices.
        /// This should be a closed polygon (i.e. first point = last point).
        struct Polygon
        {
            std::list<EAGGR::LatLong::Wgs84AccuracyPoint> vertices;
        };

        void WriteHeader(std::ostream& a_outputStream) const;
        void WriteLocations(std::ostream& a_outputStream) const;
        void WritePolygonCentres(std::ostream& a_outputStream) const;
        void WritePlacemark(
            const Placemark & a_placemark,
            const std::string & a_styleUrl,
            std::ostream& a_outputStream) const;
        void WritePolygons(std::ostream& a_outputStream) const;
        void WriteFooter(std::ostream& a_outputStream) const;

        /// Standardises a latitude coordinate so the output is the same on 32 and 64 bit builds.
        /// @param a_latitude Latitude value in degrees.
        /// @return Standardised latitude value.
        EAGGR::Utilities::Maths::Degrees StandardiseLatitude(
            const EAGGR::Utilities::Maths::Degrees a_latitude) const;

        /// Standardises a longitude coordinate so the output is the same on 32 and 64 bit builds.
        /// @param a_longitude Longitude value in degrees.
        /// @return Standardised longitude value.
        EAGGR::Utilities::Maths::Degrees StandardiseLongitude(
            const EAGGR::Utilities::Maths::Degrees a_longitude) const;

        std::list<Placemark> m_polygonCentres;
        std::list<Polygon> m_polygons;
        std::list<Placemark> m_locations;

        const EAGGR::Model::Projection::IProjection* m_pProjection;
        const EAGGR::Model::GridIndexer::IGridIndexer* m_pGridIndexer;
        const EAGGR::CoordinateConversion::CoordinateConverter m_coordinateConverter;
        const bool m_showCellCentre;
    };
  }
}
